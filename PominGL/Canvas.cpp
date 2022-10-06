#include "Graphic.h"
#include "Canvas.h"
#include "DrawState.h"
#include "MyRaster.h"
#include "MyGraphic.h"
#include "Vec.h"

void Canvas::clerarCanvas()
{
	if (g_Layer->EmptyGeometry() && g_Layer->g_Points.empty()&&g_Dataset.layerSet.size() == 1) {} // 若当前图层为空则不添加新图层
	else
	{
		for (int i = 0; i < g_Dataset.layerSet.size(); i++)
		{
			Layer* layer = g_Dataset.layerSet[i];
			layer->DeleteGeometry();
		}
		//g_Layer->DeleteGeometry();
		/*g_Layer = new Layer();
		g_Dataset.addLayer(g_Layer);*/
	}
	refreshWindow();
}

void Canvas::AddFillPoint(PixelPoint p)
{
	Point2D p2D(p.x, p.y);
	Layer* pLayer = g_Dataset.getBack();
	pLayer->g_Points.push_back(p2D);
}

void Canvas::Render()
{
	if (g_State.DrawMode == dmGrid)
	{
		Raster::drawGrid();
	}
	if (g_State.g_3DMode == Cube)
	{
		DisplaySampleCube();
		//VisibleSurfaceDetection();
	}
	//if (mInited == false) return;
	for (int i = 0; i < g_Dataset.getLayerCount(); i++)
	{
		Layer* pLayer = g_Dataset[i];
		pLayer->LayerType != "Map" ? 
			g_pRenderer->renderLayer(pLayer) : g_MapRender->renderLayer(pLayer);
	}
}

void Canvas::translate(double dx, double dy)
{
	g_pRenderer->translate(dx, dy);
	if (g_MapRender->isInitMatrix()) g_MapRender->translate(dx, dy);
}
void Canvas::scale(double dx, double dy)
{
	g_pRenderer->scale(dx, dy);
	if (g_MapRender->isInitMatrix()) g_MapRender->scale(dx, dy);
}
void Canvas::scale(double x, double y, double dx, double dy)
{
	g_pRenderer->scale(x, y, dx, dy);
	if (g_MapRender->isInitMatrix()) g_MapRender->scale(x, y, dx, dy);
}
void Canvas::rotate(double degree)
{
	g_pRenderer->rotate(degree);
	if (g_MapRender->isInitMatrix()) g_MapRender->rotate(degree);
}
void Canvas::rotate(double x, double y, double degree)
{
	g_pRenderer->rotate(x, y, degree);
	if (g_MapRender->isInitMatrix()) g_MapRender->rotate(x,y,degree);
}
void Canvas::symmetry(SymmetryType syType)
{
	g_pRenderer->symmetry(syType);
	if (g_MapRender->isInitMatrix()) g_MapRender->symmetry(syType);
}
void Canvas::symmetry(double x, double y, SymmetryType syType)
{
	g_pRenderer->symmetry(x, y, syType);
	if (g_MapRender->isInitMatrix()) g_MapRender->symmetry(x, y,syType);
}

void Canvas::Shear(double dx, double dy)
{
	g_pRenderer->Shear(dx, dy);
	if (g_MapRender->isInitMatrix()) g_MapRender->Shear(dx, dy);
}

void Canvas::InitMap()
{
	Layer* pLayer = g_Dataset[1];
	double dx = pLayer->envelop.centerX();
	double dy = pLayer->envelop.centerY();
	g_MapRender->init(&g_Dataset, dx, dy, pLayer->envelop.width(), pLayer->envelop.height(), getWindowWidth(), getWindowHeight());

}

/// <summary>
/// weiler-Atherton裁剪多边形
/// </summary>
/// <param name="clipPolygon">裁剪多边形</param>
void Canvas::WeilerAthertonClip(PolygonGeometry* clipPolygon)
{
	// 取每个图层
	for (auto layer : g_Dataset.layerSet)
	{
		if(!layer->getGeometryCount()) continue;
		vector<Geometry2D*> ClipGeo;
		int i = 0;
		// 遍历每个几何图形
		while(i < layer->getGeometryCount())
		{
			Geometry2D* psGeometry = (*layer)[i];
			if (psGeometry->getGeomType() != gtPolygon)
			{
				i++;
				continue;
			}
			vector<PolygonGeometry*> result; // 单个图形裁剪结果集合
			PolygonGeometry* geoPolygon = (PolygonGeometry*)psGeometry;
			result = Raster::WeilerAtherton(geoPolygon, clipPolygon);  // 裁剪
			if (result.size())
			{
				for (auto geo : result)
				{
					ClipGeo.push_back(geo);
				}
			}
			
			layer->DeleteGeo(i);
		}
		// 裁剪结果添加到图层中
		for (auto clip : ClipGeo)
		{
			layer->addGeometry(clip);
		}
		ClipGeo.clear();
	}
}

void Canvas::Reset()
{
	setPenColor(BLACK);
	g_MapRender->reset();
	g_pRenderer->reset();
}

void Canvas::ReverseTransMatrix(vector<Point2D>& pts)
{
	Matrix2D matrix = g_pRenderer->getInverseMatrix();
	for (int i = 0, s = pts.size(); i < s; ++i)
	{
		pts[i] = Matrix2D_Multiply(vector2D(pts[i]), matrix);
	}
}

void Canvas::DisplaySampleCube()
{
	g_3DRender->displayCube();
}

