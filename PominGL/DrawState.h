#ifndef _DRAWSTATE_H
#define _DRAWSTATE_H
#include "Graphic.h"
typedef void (*DrawModelCallBack) (int x, int y, Color color);
enum OperationType {
	otNone, otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline,
	otDrawCircle, otDrawEllipse, otDrawTriangle, otDrawTriangleOutline,
	//像素画图
	otPixel, ot10Network, ot20Network,
	// 填充
	otAreaFill,
	// 基本操作
	otPan, otRotate,
	// 裁剪
	otCai,otWeilerAtherton,

	Pan3D,
};
enum DrawModeType { dmPixel, dmGrid, dmGemotry };
enum DrawLineMethodType { dlDDA, dlMidBresenham };
enum DrawPolygonMethodType { dpXScanline, dpAET, dpEdgeFill };
enum FillMethodType { InternalPointsSeedFill, BoundarySeedFill, ScanLineSeedFill };
enum ClipMethodType { CohenSutherland, MidPoint, Sutherland_Hodgeman, };
enum SymmetryType {
	// 原点对称
	symmetryOrigin,
	// X轴对称
	symmetryX,
	// Y轴对称
	symmetryY,
	// 第一象限对称轴对称
	symmetryXY,
	// 第二象限对称轴对称
	symmetryYX
};
enum Base3DMode
{
	Cube,
	None,
};
enum State3D
{
	Blanking,
	Texture,
	LightSimple,
	LightGourand,
	NoneState,
};
struct State
{
	OperationType g_OperationType = otNone;
	DrawModelCallBack drawPixelCB = setPixel;
	// 绘制模式
	DrawModeType DrawMode;
	int GridWidth = 10; // 网格宽
	int GridHeight = 10; // 网格高

	Color GridColor1 = WHITE;
	Color GridColor2 = GREEN;

	// 画线方法
	DrawLineMethodType DrawLineMethod;
	// 多边形填充算法
	DrawPolygonMethodType DrawPolygonMethod;
	// 裁剪算法
	ClipMethodType ClipMethod;
	// 边表节点
	struct tagEDGE {
		double x;
		double dx;
		int ymax;
	};

	// 区域填充算法

	FillMethodType FillMethod;

	SymmetryType symmetryMethod;

	Base3DMode g_3DMode;
	State3D g_3DState;
	State();
};
extern State g_State;
#endif // !_DRAWSTATE_H
