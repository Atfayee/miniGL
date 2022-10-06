#ifndef _DRAWSTATE_H
#define _DRAWSTATE_H
#include "Graphic.h"
typedef void (*DrawModelCallBack) (int x, int y, Color color);
enum OperationType {
	otNone, otDrawRectangle, otDrawRectangleOutline,
	otDrawLine, otDrawPolyline, otDrawPolygon, otDrawPolygonOutline,
	otDrawCircle, otDrawEllipse, otDrawTriangle, otDrawTriangleOutline,
	//���ػ�ͼ
	otPixel, ot10Network, ot20Network,
	// ���
	otAreaFill,
	// ��������
	otPan, otRotate,
	// �ü�
	otCai,otWeilerAtherton,

	Pan3D,
};
enum DrawModeType { dmPixel, dmGrid, dmGemotry };
enum DrawLineMethodType { dlDDA, dlMidBresenham };
enum DrawPolygonMethodType { dpXScanline, dpAET, dpEdgeFill };
enum FillMethodType { InternalPointsSeedFill, BoundarySeedFill, ScanLineSeedFill };
enum ClipMethodType { CohenSutherland, MidPoint, Sutherland_Hodgeman, };
enum SymmetryType {
	// ԭ��Գ�
	symmetryOrigin,
	// X��Գ�
	symmetryX,
	// Y��Գ�
	symmetryY,
	// ��һ���޶Գ���Գ�
	symmetryXY,
	// �ڶ����޶Գ���Գ�
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
	// ����ģʽ
	DrawModeType DrawMode;
	int GridWidth = 10; // �����
	int GridHeight = 10; // �����

	Color GridColor1 = WHITE;
	Color GridColor2 = GREEN;

	// ���߷���
	DrawLineMethodType DrawLineMethod;
	// ���������㷨
	DrawPolygonMethodType DrawPolygonMethod;
	// �ü��㷨
	ClipMethodType ClipMethod;
	// �߱�ڵ�
	struct tagEDGE {
		double x;
		double dx;
		int ymax;
	};

	// ��������㷨

	FillMethodType FillMethod;

	SymmetryType symmetryMethod;

	Base3DMode g_3DMode;
	State3D g_3DState;
	State();
};
extern State g_State;
#endif // !_DRAWSTATE_H
