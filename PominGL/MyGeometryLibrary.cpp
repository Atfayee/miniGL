#include "MyGeometryLibrary.h"
#include "Graphic.h"
#include "MyRaster.h"

Geometry2D* GeometryLibrary::createPointGeometry(double x, double y)
{
	return new PointGeometry(x, y);
}

Geometry2D* GeometryLibrary::createPolylineGeometry(PixelPoint* pts, int size, unsigned color)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	pGeometry->color = color;
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolylineGeometry(Point2D* pts, int size)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolylineGeometry(PixelPoint& pt1, PixelPoint& pt2, unsigned color)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	pGeometry->addPoint(pt1.x, pt1.y);
	pGeometry->addPoint(pt2.x, pt2.y);
	pGeometry->color = color;
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolygonGeometry(PixelPoint* pts, int size)
{
	PolygonGeometry* pGeometry = new PolygonGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	return pGeometry;
}

Geometry2D* GeometryLibrary::createTriangleGemoetry(PixelPoint* pts, int size)
{
	PolygonGeometry* pGeometry = new PolygonGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolygonOutlineGeometry(PixelPoint* pts, int size, unsigned color)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	pGeometry->addPoint(pts[0].x, pts[0].y);
	pGeometry->color = color;
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolygonGeometry(Point2D* pts, int size)
{
	PolygonGeometry* pGeometry = new PolygonGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	return pGeometry;
}

Geometry2D* GeometryLibrary::createPolygonOutlineGeometry(Point2D* pts, int size)
{
	PolylineGeometry* pGeometry = new PolylineGeometry();
	for (int i = 0; i < size; ++i)
	{
		pGeometry->addPoint(pts[i].x, pts[i].y);
	}
	pGeometry->addPoint(pts[0].x, pts[0].y);
	return pGeometry;
}

Geometry2D* GeometryLibrary::createCircleGeometry(double x, double y, double r)
{
	return new CircleGeometry(x, y, r);
}

Geometry2D* GeometryLibrary::createCircleGeometry(double x1, double y1, double x2, double y2)
{
	double dx = x1 - x2;
	double dy = y1 - y2;
	return new CircleGeometry(x1, y1, sqrt(dx * dx + dy * dy));
}

Geometry2D* GeometryLibrary::createCircleGeometry(double x1, double y1, double x2, double y2, unsigned color)
{
	double dx = x1 - x2;
	double dy = y1 - y2;
	return new CircleGeometry(x1, y1, sqrt(dx * dx + dy * dy), color);
}

Geometry2D* GeometryLibrary::createEllipseGeometry(double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);

	return new EllipseGeometry(x1, y1, x2, y2);
}

Geometry2D* GeometryLibrary::createEllipseGeometry(double x1, double y1, double x2, double y2, unsigned color)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);

	return new EllipseGeometry(x1, y1, x2, y2, color);
}

Geometry2D* GeometryLibrary::creatRectangleGeometry(double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);

	PolygonGeometry* pGeometry = new PolygonGeometry();
	pGeometry->addPoint(x1, y1);
	pGeometry->addPoint(x2, y1);
	pGeometry->addPoint(x2, y2);
	pGeometry->addPoint(x1, y2);
	return pGeometry;
}

Geometry2D* GeometryLibrary::creatRectangleOutlineGeometry(double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);

	PolygonGeometry* pGeometry = new PolygonGeometry();
	pGeometry->addPoint(x1, y1);
	pGeometry->addPoint(x2, y1);
	pGeometry->addPoint(x2, y2);
	pGeometry->addPoint(x1, y2);
	return pGeometry;
}


void ClipTool::LButtonUp()
{
	PixelPoint pc[2];
	getRubberPoints(pc);
	vector<Point2D> pts;
	pts.push_back(Point2D(pc[0].x,pc[0].y));
	pts.push_back(Point2D(pc[1].x,pc[1].y));
	g->ReverseTransMatrix(pts);
	pc[0].x = pts[0].x, pc[0].y = pts[0].y;
	pc[1].x = pts[1].x, pc[1].y = pts[1].y;
	switch (g_State.ClipMethod)
	{
	case CohenSutherland:
		Raster::CohenSutherland(pc[0], pc[1], g->getBack());
		refreshWindow();
		break;
	case MidPoint:
		Raster::MidPointClipAlgorithm(pc[0], pc[1], g->getBack());
		refreshWindow();
		break;
	case Sutherland_Hodgeman:
		Raster::Sutherland_Hodgeman(pc[0], pc[1], g->getBack());
		refreshWindow();
		break;
	}
}

void Drag3DTool::LButtonDown(int x, int y)
{
	lbuttondown[0] = x, lbuttondown[1] = y;
	
}

void Drag3DTool::MouseMove(int x, int y)
{
	mousemove[0] = x, mousemove[1] = y;

}

void Drag3DTool::LButtonUp()
{

}


void Pan3DTool::LButtonDown(int x, int y)
{
	if (!rotate)
	{
		leftmousedown.x = x;
		leftmousedown.y = y;
		rotate = true;
		return;
	}
	//drag = false;
}

void Pan3DTool::LButtonUp(int x, int y)
{
	if (drag) drag = false;
	leftmouseup.x = x; leftmouseup.y = y;
	if (rotate) rotate = false;
}

void Pan3DTool::RButtonDown(int x, int y)
{
	if (!drag)
	{
		leftmousedown.x = 0;
		leftmousedown.y = 0;
		drag = true;
		return;
	}
	//drag = false;
}

void Pan3DTool::RButtonUp()
{
	if (drag) drag = false;
}

void Pan3DTool::MouseMove(int x, int y)
{
	mousemove.x = x;
	mousemove.y = y;
	if (drag)
	{
		g->g_3DRender->translate((mousemove.x - leftmousedown.x)*2/screen_width, (mousemove.y - leftmousedown.y)*2/screen_height,0);
		return;
	}
	if (rotate)
	{
		RotateSphere(leftmousedown.x, leftmousedown.y, mousemove.x, mousemove.y);
		return;
	}
}


void Pan3DTool::RotateSphere(double sX, double sY, double eX, double eY)
{
	double angle = 0.05;
	vector3D startPos = { sX*2/screen_width,sY*2/screen_height ,g->g_3DRender->getEyeCoord().z };
	vector3D endPos = { eX*2/screen_width ,eY*2/screen_height,g->g_3DRender->getEyeCoord().z };
	vector3D RotateAxis = startPos.vector_crossMutiply(endPos);
	double cosa = abs(startPos.vector_dotMutiply(endPos)) / (startPos.getVectorLength() * endPos.getVectorLength());
	angle = acos(cosa);
	RotateAxis.y *= -1;
	g->g_3DRender->rotate(RotateAxis, angle);
}

//Geometry2D* GeometryLibrary::createTriangleGemoetry(PixelPoint* pts, int size, unsigned color) {
//	PolygonGeometry* pGeometry = new PolygonGeometry();
//	for (int i = 0; i < 3; ++i)
//	{
//		pGeometry->addPoint(pts[i].x, pts[i].y);
//	}
//	pGeometry->addPoint(pts[0].x, pts[0].y);
//	pGeometry->color = color;
//	return pGeometry;
//}
//
//