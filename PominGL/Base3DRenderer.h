#pragma once
#include "Matrix.h"
#include "Vec.h"
#include "Graphic.h"
#include "FractalImageGenerator.h"

#define PMAXW 1200  //范围宽度
#define PMAXH 1200  //范围高度

const int INF = 9999;        //深度值

static vector3D L(2, 2, 2);

class Base3DRender
{
public:
	// 定义缓存器
	typedef struct gPixelBuc {
		Color color = 99;
		double depth = -9999;
	};

	gPixelBuc g_PixelBuc[PMAXW][PMAXH];

	// 定义三角形面拓扑结构
	struct tpsFace {
		vector3D point3D[3];
		int size = 3;
		vector3D operator[](int i)
		{
			return point3D[i];
		}
	}CubeFace[12];//正方体有12个三角形组成的面2*6=12

	
	double CubeScale = 1; // 整体缩放
	double dx=0, dy=0, dz=0; // 平移距离
	bool isLRtoUD = false; // 相机旋转方向上下或左右
	bool isCheckBack = false; // 是否开启背面剔除
	vector3D Xaxis; // 相机旋转轴
	int texture_grid = 1; // 纹理最大网格数

	bool rotateLorE = true; // true移动相机视点

	Base3DRender();
	// 初始化变换矩阵
	// transform = world * view *  proj
	void transform_init();
	// 变换矩阵更新
	void transform_update();
	// 设置观察坐标系
	void setCameraAtZero(vector3D eye);
	void transform_normalize(vector3D& vec);

	// 绘制三角形
	void drawTraingle(vector3D& p1, vector3D& p2, vector3D& p3,Color color);
	void drawTraingle(texture_v& p1, texture_v& p2, texture_v& p3);
	void drawTraingle(tpsFace& _cubeFace);

	// 绘制面
	void drawPlane(int a, int b, int c, int d,Color color);

	// 绘制立方体
	void displayCube();
	void translate(double dx, double dy, double dz);
	void rotate(vector3D axis, double angle);
	void Scale(double sX);

	// 3D环绕相机设置
	void eye_Rotate_Update(); // 相机环绕轴更新
	void set_eye_Angle(double arc); // 设置相机旋转角度
	void set_eye_far(double dis); // 设置相机距离远近


	vector3D getEyeCoord();
	// 建立三角面
	void ptsConvertToFace(vector3D(&pts_)[8], tpsFace(&faces)[12]);

	// 设置移动相机视点或者光源
	void setRotateLight(bool isL);

	void CalLightIntensity(tpsFace(&faces)[12]);

	void setRGBv(double r, double g, double b);

	void reset3D();
private:
	Matrix3D mWorld;
	Matrix3D mView;
	Matrix3D mProj;
	vector3D eye = { 0,0,7 };
	double w = 600; // 屏幕宽
	double h = 600; // 屏幕高

	Matrix3D mTransform;
	Matrix3D mRotate; // 物体旋转矩阵
	// 立方体顶点坐标

	
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
	// 带纹理坐标,带颜色以及光照强度的顶点,绘制的时候再具体赋值
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

	// 带颜色以及光照强度的顶点




};
