#include "Map2DRenderer.h"

Map2DRenderer::Map2DRenderer() {}
//************************************
// Method:    init
// FullName:  Map2DRenderer::init
// Access:    public
// Returns:   void
// Qualifier:
// Parameter: Dataset * pDataset
// Parameter: double xOrig_map
// Parameter: double yOrig_map
// Parameter: double width_map
// Parameter: double height_map
// Parameter: int width_wnd
// Parameter: int height_wnd
//************************************
void Map2DRenderer::init(Dataset* pDataset, double xOrig_map, double yOrig_map,
	double width_map, double height_map, int width_wnd,
	int height_wnd)
{
	mTransformMatrix *= Matrix2D::Translate(-xOrig_map, -yOrig_map);
	double s = _calcScale(width_map, height_map, width_wnd, height_wnd);
	mTransformMatrix *= Matrix2D::Scale(s);
	preTransformMatrix = mTransformMatrix;
	Minit = true;
}
void Map2DRenderer::reset()
{
	mTransformMatrix = preTransformMatrix;
}
/// <summary>
/// 计算比例尺
/// </summary>
/// <param name="mapWidth">地图宽</param>
/// <param name="mapHeight">地图高</param>
/// <param name="wndWidth">窗口宽</param>
/// <param name="wndHeight">窗口高</param>
/// <returns></returns>
double Map2DRenderer::_calcScale(double mapWidth, double mapHeight, 
	double wndWidth, double wndHeight)
{
	double sX = wndWidth / mapWidth;
	double sY = wndHeight / mapHeight;
	if (sX < sY) return sX;
	else return sY;
}