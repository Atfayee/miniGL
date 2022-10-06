#pragma once
#include "Matrix.h"
#include "Vec.h"
#include "Graphic.h"
#include "FractalImageGenerator.h"

#define PMAXW 1200  //��Χ���
#define PMAXH 1200  //��Χ�߶�

const int INF = 9999;        //���ֵ

static vector3D L(2, 2, 2);

class Base3DRender
{
public:
	// ���建����
	typedef struct gPixelBuc {
		Color color = 99;
		double depth = -9999;
	};

	gPixelBuc g_PixelBuc[PMAXW][PMAXH];

	// ���������������˽ṹ
	struct tpsFace {
		vector3D point3D[3];
		int size = 3;
		vector3D operator[](int i)
		{
			return point3D[i];
		}
	}CubeFace[12];//��������12����������ɵ���2*6=12

	
	double CubeScale = 1; // ��������
	double dx=0, dy=0, dz=0; // ƽ�ƾ���
	bool isLRtoUD = false; // �����ת�������»�����
	bool isCheckBack = false; // �Ƿ��������޳�
	vector3D Xaxis; // �����ת��
	int texture_grid = 1; // �������������

	bool rotateLorE = true; // true�ƶ�����ӵ�

	Base3DRender();
	// ��ʼ���任����
	// transform = world * view *  proj
	void transform_init();
	// �任�������
	void transform_update();
	// ���ù۲�����ϵ
	void setCameraAtZero(vector3D eye);
	void transform_normalize(vector3D& vec);

	// ����������
	void drawTraingle(vector3D& p1, vector3D& p2, vector3D& p3,Color color);
	void drawTraingle(texture_v& p1, texture_v& p2, texture_v& p3);
	void drawTraingle(tpsFace& _cubeFace);

	// ������
	void drawPlane(int a, int b, int c, int d,Color color);

	// ����������
	void displayCube();
	void translate(double dx, double dy, double dz);
	void rotate(vector3D axis, double angle);
	void Scale(double sX);

	// 3D�����������
	void eye_Rotate_Update(); // ������������
	void set_eye_Angle(double arc); // ���������ת�Ƕ�
	void set_eye_far(double dis); // �����������Զ��


	vector3D getEyeCoord();
	// ����������
	void ptsConvertToFace(vector3D(&pts_)[8], tpsFace(&faces)[12]);

	// �����ƶ�����ӵ���߹�Դ
	void setRotateLight(bool isL);

	void CalLightIntensity(tpsFace(&faces)[12]);

	void setRGBv(double r, double g, double b);

	void reset3D();
private:
	Matrix3D mWorld;
	Matrix3D mView;
	Matrix3D mProj;
	vector3D eye = { 0,0,7 };
	double w = 600; // ��Ļ��
	double h = 600; // ��Ļ��

	Matrix3D mTransform;
	Matrix3D mRotate; // ������ת����
	// �����嶥������

	
	vector3D Cube[8] = {
		{1,-1,1},
		{-1,-1,1},
		{-1,1,1},
		{1,1,1},
		{1,-1,-1},
		{-1,-1,-1},
		{-1,1,-1},
		{1,1,-1}
	};
	// ����������,����ɫ�Լ�����ǿ�ȵĶ���,���Ƶ�ʱ���پ��帳ֵ
	texture_v CubeNew[8] = {
		{Cube[0],0,0,255,51,51,0},
		{Cube[1],0,1,51,255,51,0},
		{Cube[2],1,1,51,51,255,0},
		{Cube[3],1,0,255,51,255,0},
		{Cube[4],0,0,255,255,51,0},
		{Cube[5],0,1,51,255,255,0},
		{Cube[6],1,1,255,76.5,76.5,0},
		{Cube[7],1,0,51,255,76.5,0}
	};

	// ����ɫ�Լ�����ǿ�ȵĶ���




};
