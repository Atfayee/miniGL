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
	//�������ݼ�������ͼ��
	//virtual void render();

	////��ԭ
	virtual void reset();

	virtual Matrix2D getInverseMatrix();

	////ƽ�ƣ�������Ϊ��λ
	void translate(double dx, double dy);

	////��ת����ʱ��Ϊ���� �Զ�Ϊ��λ
	void rotate(double degree);

	////������ת����ʱ��Ϊ���� �Զ�Ϊ��λ
	void rotate(double x, double y, double degree);

	////���ķ���
	void scale(double dx, double dy);

	////�������
	void scale(double x, double y, double dx, double dy);

	// �ԳƱ任
	void symmetry(SymmetryType syType);
	void symmetry(double x, double y, SymmetryType syType);
	// ���б任
	void Shear(double dx, double dy);

protected:
	//����һ��ͼ��

	virtual void renderGeometry(Geometry* pGeometryDef, Color color);

	virtual unsigned getLayerColor(Layer* pLayer);

	//virtual void renderLayerBBox( Layer* pLayer );
	Matrix2D mTransformMatrix;//�任����
	vector<Matrix2D> transStates;
	bool Minit = false;
};

#endif
