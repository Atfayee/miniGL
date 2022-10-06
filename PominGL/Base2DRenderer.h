#ifndef _BASE2DRENDERER_H
#define _BASE2DRENDERER_H
#include "Graphic.h"
#include "Matrix.h"
#include "DrawState.h"
#include <vector>


struct Geometry;
struct Dataset;
struct Layer;

class Base2DRenderer
{
public:
	Base2DRenderer(void);
	virtual ~Base2DRenderer(void);
	virtual void renderLayer(Layer* pLayer);
	//绘制数据集包含的图层
	//virtual void render();

	////复原
	virtual void reset();

	virtual Matrix2D getInverseMatrix();

	////平移，以像素为单位
	void translate(double dx, double dy);

	////旋转，逆时针为正， 以度为单位
	void rotate(double degree);

	////定点旋转，逆时针为正， 以度为单位
	void rotate(double x, double y, double degree);

	////中心放缩
	void scale(double dx, double dy);

	////定点放缩
	void scale(double x, double y, double dx, double dy);

	// 对称变换
	void symmetry(SymmetryType syType);
	void symmetry(double x, double y, SymmetryType syType);
	// 错切变换
	void Shear(double dx, double dy);

protected:
	//绘制一个图层

	virtual void renderGeometry(Geometry* pGeometryDef, Color color);

	virtual unsigned getLayerColor(Layer* pLayer);

	//virtual void renderLayerBBox( Layer* pLayer );
	Matrix2D mTransformMatrix;//变换矩阵
	vector<Matrix2D> transStates;
	bool Minit = false;
};

#endif
