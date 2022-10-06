#ifndef _MAP2DRENDERER_H
#define _MAP2DRENDERER_H

#include "Matrix.h"
#include "Base2DRenderer.h"

class Map2DRenderer : public Base2DRenderer
{
public:
	Map2DRenderer();

	void init(Dataset* pDataset, double xOrig_map, double yOrig_map,
		double width_map, double height_map, int width_wnd, int height_wnd);

	void reset();
	bool isInitMatrix()
	{
		if (Minit) return true;
		return false;
	}

	/*void scale(double d);

	void scale(double x, double y, double d);*/

protected:

	double _calcScale(double mapWidth, double mapHeight, double wndWidth, double wndHeight);

	int mWndWidth, mWndHeight;//���ڿ��
	double mXOrig_map, mYOrig_map;//��Ӧ����ԭ��ĵ�ͼ��
	double mMapWidth, mMapHeight;//��ͼ���

	Matrix2D preTransformMatrix;
};

#endif
