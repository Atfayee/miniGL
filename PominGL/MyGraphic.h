/******************************
*file base:MyGraphic.cpp
*编写图形光栅化算法的接口
*******************************/
#ifndef _MYGRAPHIC_H
#define _MYGRAPHIC_H
#include "Graphic.h"
#include <vector>
#include "DrawState.h"
#include "MyGeoDefine.h"
using namespace std;

/*线的光栅化--中点Bresenham算法*/
void MidPointLine(double x0, double y0, double x1, double y1, unsigned color);
void DDALine(double x0, double y0, double x1, double y1, unsigned color);

/*画圆八个点对应位置*/
void CirclePlot(int x, int y, int xa, int ya, unsigned color);

/*画椭圆四个对应点*/
void EllipsePlot(int xCenter, int yCenter, int x, int y, unsigned color);

/*水平线直接填充*/
void HorizonEdgeFill(PixelPoint* data, int size, unsigned color);
void GetYMinMax(PixelPoint* data, int size, int& ymin, int& ymax);
/*初始化新边表*/
void InitET(vector<vector<State::tagEDGE>>& etEDGE, PixelPoint* data, int size, int ymin, int ymax);
/*初始化活动边表*/
void InitAET();
/*多边形扫描填充*/
void ScanLineFill(vector<State::tagEDGE>& aetEDGE, vector<vector<State::tagEDGE>>& etEDGE, int ymin, int ymax, unsigned color);

/*区域填充算法*/

/// <summary>
/// 内部点集定义区域填充算法
/// </summary>
/// <param name="x">种子点x坐标</param>
/// <param name="y">种子点y坐标</param>
/// <param name="newColor">用于填充区域的新颜色</param>
void PointFill(int x, int y, unsigned newColor);

/// <summary>
/// 边界定义区域填充算法
/// </summary>
/// <param name="x">种子点x坐标</param>
/// <param name="y">种子点y坐标</param>
/// <param name="bdrColor">边界点颜色</param>
/// <param name="newColor">用于填充的新颜色</param>
void BoundaryFill(int x, int y, unsigned bdrColor, unsigned newColor);

/// <summary>
/// 扫描线种子填充算法
/// </summary>
/// <param name="x">种子点x坐标</param>
/// <param name="y">种子点y坐标</param>
/// <param name="bdrColor">边界颜色</param>
/// <param name="newColor">填充颜色</param>
void ScanLineAreaFill(int x, int y, unsigned bdrColor, unsigned newColor);

// 求两点距离
double distance(Point2D p1, Point2D p2);
#endif // !_MYGRAPHIC_H
