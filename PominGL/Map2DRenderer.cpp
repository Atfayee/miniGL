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
/// ���������
/// </summary>
/// <param name="mapWidth">��ͼ��</param>
/// <param name="mapHeight">��ͼ��</param>
/// <param name="wndWidth">���ڿ�</param>
/// <param name="wndHeight">���ڸ�</param>
/// <returns></returns>
double Map2DRenderer::_calcScale(double mapWidth, double mapHeight, 
	double wndWidth, double wndHeight)
{
	double sX = wndWidth / mapWidth;
	double sY = wndHeight / mapHeight;
	if (sX < sY) return sX;
	else return sY;
}