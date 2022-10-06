#pragma once
#include "MyGeoDefine.h"
#include "DrawState.h"
#include "Base2DRenderer.h"
#include "Map2DRenderer.h"
#include "Base3DRenderer.h"
class Canvas
{
private:
	Dataset g_Dataset;
	Layer* g_Layer = NULL;
	Base2DRenderer* g_pRenderer = NULL;
	Map2DRenderer* g_MapRender = NULL;
	//Base3DRender* g_3DRender = NULL;
public:
	Base3DRender* g_3DRender = NULL;
	Canvas()
	{
		g_Layer = new Layer();
		g_Dataset.addLayer(g_Layer);
		g_pRenderer = new Base2DRenderer();
		g_MapRender = new Map2DRenderer();
		g_3DRender = new Base3DRender();
	}

	void AddGeometry(Geometry2D* Geo)
	{
		g_Layer->addGeometry(Geo);
	}
	void AddLayer(Layer* ly)
	{
		g_Dataset.addLayer(ly);
	}
	int GetLayerCount()
	{
		return g_Dataset.getLayerCount();
	}
	Layer* getBack()
	{
		return g_Dataset.layerSet.back();
	}
	void InitMap();
	void Render();
	void ReverseTransMatrix(vector<Point2D>& pts);
	// 平移
	void translate(double dx, double dy);
	// 中心缩放
	void scale(double dx, double dy);
	// 定点缩放
	void scale(double x, double y, double dx, double dy);
	// 中心旋转
	void rotate(double degree);
	// 定点旋转
	void rotate(double x, double y, double degree);
	// 对称
	void symmetry(SymmetryType syType);
	void symmetry(double x, double y, SymmetryType syType);
	// 错切
	void Shear(double dx, double dy);

	void DisplaySampleCube();
	// 重置
	void Reset();
	void clerarCanvas();
	void AddFillPoint(PixelPoint p);
	void WeilerAthertonClip(PolygonGeometry* clipPolygon);

};
