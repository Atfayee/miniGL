#include "Base2DRenderer.h"
#include "MyGeoDefine.h"
#include "Graphic.h"
#include "MyGraphic.h"
#include "MyRaster.h"
#include "DrawState.h"

Base2DRenderer::Base2DRenderer(void)
{
}

Base2DRenderer::~Base2DRenderer(void)
{
}

void Base2DRenderer::renderLayer(Layer* pLayer)
{
	if (pLayer->EmptyGeometry()) return;
	unsigned color = getLayerColor(pLayer);

	for (int i = 0, size = pLayer->getGeometryCount(); i < size; ++i)
	{
		renderGeometry((*pLayer)[i], color);
	}
	if (!pLayer->g_Points.empty())
	{
		for (int i = 0; i < pLayer->g_Points.size(); i++)
		{
			PixelPoint pt = Matrix2D_Multiply(vector2D(pLayer->g_Points[i]), mTransformMatrix);
			switch (g_State.FillMethod)
			{
			case InternalPointsSeedFill:
				PointFill(pt.x, pt.y, YELLOW);
				break;
			case BoundarySeedFill:
				BoundaryFill(pt.x, pt.y, BLACK, YELLOW);
				break;
			case ScanLineSeedFill:
				ScanLineAreaFill(pt.x, pt.y, BLACK, YELLOW);
				break;
			default:
				return;
			}
		}
	}
}

void Base2DRenderer::renderGeometry(Geometry* pGeometryDef, Color color)
{
	switch (pGeometryDef->getGeomType())
	{
	case gtPoint:
	{
		PointGeometry* pGeometry = (PointGeometry*)pGeometryDef;
		Raster::drawPoint(pGeometry->x, pGeometry->y, getPenColor());
	}
	break;
	case gtPolyline:
	{
		PolylineGeometry* pGeometry = (PolylineGeometry*)pGeometryDef;
		const vector<Point2D>& pts = pGeometry->getPts();
		static vector<PixelPoint> rasterPts;
		rasterPts.resize(pts.size());
		for (int i = 0, s = pts.size(); i < s; ++i)
		{
			rasterPts[i] = Matrix2D_Multiply(vector2D(pts[i]), mTransformMatrix);
		}
		if (g_State.DrawMode == dmGrid)
			Raster::ChangeOf_XYs(rasterPts.data(), rasterPts.size());
		Raster::drawPolyline(rasterPts.data(), rasterPts.size(), pGeometry->color);
	}
	break;
	case gtPolygon:
	{
		//setOrig(0, getWindowHeight());
		PolygonGeometry* pGeometry = (PolygonGeometry*)pGeometryDef;
		const vector<Point2D>& pts = pGeometry->getPts();
		vector<PixelPoint> rasterPts;
		rasterPts.resize(pts.size());
		for (int i = 0, s = pts.size(); i < s; ++i)
			rasterPts[i] = Matrix2D_Multiply(vector2D(pts[i]), mTransformMatrix);
		if (g_State.DrawMode == dmGrid)
			Raster::ChangeOf_XYs(rasterPts.data(), rasterPts.size());
		Raster::drawPolygon(rasterPts.data(), rasterPts.size(), pGeometry->color);
		//Raster::drawPolyline(rasterPts.data(), rasterPts.size(), pGeometry->color);
	}
	break;
	case gtTriangle:
	{
		PolygonGeometry* pGeometry = (PolygonGeometry*)pGeometryDef;
		const vector<Point2D>& pts = pGeometry->getPts();
		vector<PixelPoint> rasterPts;
		rasterPts.resize(pts.size());
		for (int i = 0, s = pts.size(); i < s; ++i)
			rasterPts[i] = Matrix2D_Multiply(vector2D(pts[i]), mTransformMatrix);
		if (g_State.DrawMode == dmGrid)
			Raster::ChangeOf_XYs(rasterPts.data(), rasterPts.size());
		Raster::drawTriangle(rasterPts.data(), rasterPts.size());
	}
	break;
	case gtCircle:
	{
		CircleGeometry* pGeometry = (CircleGeometry*)pGeometryDef;
		Point2D center = pGeometry->GetCenter();
		double r = pGeometry->r;
		if (Matrix2D::TransformType == "scale") r *= mTransformMatrix.matrix[0][0];
		//Matrix2D::TransformType = "none";
		PixelPoint Center = Matrix2D_Multiply(vector2D(center), mTransformMatrix);
		//Raster::drawCircle(pGeometry->x, pGeometry->y, pGeometry->r, pGeometry->color);
		Raster::drawCircle(Center.x, Center.y, r, pGeometry->color);
	}
	break;

	case gtEllipse:
	{
		EllipseGeometry* pGeometry = (EllipseGeometry*)pGeometryDef;
		Point2D center;
		//double x, y;
		pGeometry->getCenter(center.x, center.y);
		double width = pGeometry->getWidth(), height = pGeometry->getHeight();
		if (Matrix2D::TransformType == "scale")
		{
			width *= mTransformMatrix.matrix[0][0];
			height *= mTransformMatrix.matrix[0][0];
		}
		//Matrix2D::TransformType = "none";
		PixelPoint Center = Matrix2D_Multiply(vector2D(center), mTransformMatrix);
		Raster::drawEllipse(Center.x, Center.y, width, height, pGeometry->color);
	}
	break;
	}
}

unsigned Base2DRenderer::getLayerColor(Layer* pLayer)
{
	return getPenColor();
}

void Base2DRenderer::reset()
{
	mTransformMatrix.InitMatrix();
}
void Base2DRenderer::translate(double dx, double dy)
{
	mTransformMatrix = mTransformMatrix * Matrix2D::Translate(dx, dy);
	transStates.push_back(Matrix2D::Translate(dx, dy));
}

void Base2DRenderer::rotate(double degree)
{
	//double rad = degree * PI / 180;
	mTransformMatrix *= Matrix2D::Rotate(degree);
	transStates.push_back(Matrix2D::Rotate(degree));
}

void Base2DRenderer::rotate(double x, double y, double degree)
{
	mTransformMatrix *= Matrix2D::Translate(-x, -y);
	rotate(degree);
	mTransformMatrix *= Matrix2D::Translate(x, y);
}

void Base2DRenderer::scale(double dx, double dy)
{
	mTransformMatrix *= Matrix2D::Scale(dx);
	transStates.push_back(Matrix2D::Scale(dx));
}

void Base2DRenderer::scale(double x, double y, double dx, double dy)
{
	mTransformMatrix *= Matrix2D::Translate(-x, -y);
	scale(dx, dy);
	mTransformMatrix *= Matrix2D::Translate(x, y);
}

void Base2DRenderer::symmetry(SymmetryType syType)
{
	mTransformMatrix *= Matrix2D::Symmetry(syType);
	transStates.push_back(Matrix2D::Symmetry(syType));
}
void Base2DRenderer::symmetry(double x, double y, SymmetryType syType)
{
	mTransformMatrix *= Matrix2D::Translate(-x, -y);
	symmetry(syType);
	mTransformMatrix *= Matrix2D::Translate(x, y);
}

void Base2DRenderer::Shear(double dx, double dy)
{
	mTransformMatrix *= Matrix2D::Shear(dx, dy);
	transStates.push_back(Matrix2D::Shear(dx, dy));
}

Matrix2D getMatrixRe(Matrix2D& matrix)
{
	Matrix2D NewMatrix;
	switch (matrix.transformType)
	{
	case 0:
		NewMatrix.matrix[2][0] = matrix.matrix[2][0] * -1;
		NewMatrix.matrix[2][1] = matrix.matrix[2][1] * -1;
		break;
	case 1:
	{
		NewMatrix.matrix[0][0] = matrix.matrix[0][0];
		NewMatrix.matrix[1][1] = matrix.matrix[1][1];
		NewMatrix.matrix[0][1] = matrix.matrix[1][0];
		NewMatrix.matrix[1][0] = matrix.matrix[0][1];
		break;
	}
	case 2:
		NewMatrix.matrix[0][0] = 1 / matrix.matrix[0][0];
		NewMatrix.matrix[1][1] = 1 / matrix.matrix[1][1];
		break;
	case 3:
		break;
	}
	return NewMatrix;
}

Matrix2D Base2DRenderer::getInverseMatrix()
{
	Matrix2D inverseMatrix;
	if (!transStates.size()) return inverseMatrix;
	for (int i = (transStates.size() - 1); i >= 0; i--)
	{
		inverseMatrix *= getMatrixRe(transStates[i]);
	}
	return inverseMatrix;
}