#include <windows.h>
#include <opencv2/opencv.hpp>
#include "MessageHandler.h"
#include "Resource.h"
#include "Graphic.h"
#include "MyGraphic.h"
#include "MyGeoDefine.h"
#include "MyGeometryLibrary.h"
#include "MyRaster.h"
#include "DrawState.h"
#include "Canvas.h"
#include "CreateToolFactory.h"
#include "Utils.h"

using namespace std;
//using namespace cv;
string Matrix2D::TransformType = "none";
boolean Reset = false;
// ��ȡ��ǰģʽ
RubberMode rm;
// ����һ��ͼ��ָ��
Canvas* g_ctx = NULL;
// ����һ����ʼ������
Tool* action_tool = NULL;
int Raster::texture_w = 256;
int Raster::texture_h = 256;
unsigned* Raster::pImage = NULL;
unsigned** Raster::pImageHeader = NULL;
unsigned* Raster::pHeader = NULL;
unsigned Raster::texture[256][256];
int Raster::texture_mode = 0;

// ��ʼ������
void Initialize()
{
	g_ctx = new Canvas();
}

///����˵���Ϣ
void handleMenuMessage(int menuID)
{
	//Mat img = imread("E:\\360�������������\\Homework\\�Ļ���ҵ����\\��ҵ�ƻ������.jpg");
	switch (menuID)
	{
	case ID_DRAWMODE_GRID_10:
	{
		g_State.DrawMode = dmGrid;
		g_State.drawPixelCB = Raster::drawCell;
		refreshWindow();
		break;
	}

	case ID_DRAWMODE_PIXEL:
	{
		g_State.DrawMode = dmPixel;
		g_State.g_OperationType = otPixel;
		setRubberMode(rmNone);
		g_State.drawPixelCB = setPixel;

		refreshWindow();
		break;
	}

	case ID_DRAWMODE_GRID_20:
	{
		g_State.DrawMode = dmGrid;
		g_State.GridHeight = 20;
		g_State.GridWidth = 20;
		g_State.drawPixelCB = Raster::drawCell;
		refreshWindow();
		break;
	}
	case ID_DRAWMODEL_GEOMETRY:
	{
		g_State.DrawMode = dmGemotry;
		g_State.drawPixelCB = setPixel;
		refreshWindow();
		break;
	}
	case ID_2D__RECTANGLE_OUTLINE:
	{
		setCursor(csSize);
		setRubberMode(rmRectangle);
		g_State.g_OperationType = otDrawRectangleOutline;
		refreshWindow();
		break;
	}
	case ID_2D_LINE_DDA:
	{
		setCursor(csArrow);
		setRubberMode(rmLine);
		g_State.DrawLineMethod = dlDDA;
		g_State.g_OperationType = otDrawLine;
		break;
	}
	case ID_2D_LINE_MIDBRESENHAM:
		setCursor(csArrow);
		setRubberMode(rmLine);
		g_State.DrawLineMethod = dlMidBresenham;
		g_State.g_OperationType = otDrawLine;
		break;
	case ID_2D_POLYLINE:
	{
		setCursor(csHand);
		setRubberMode(rmPolyline);
		g_State.g_OperationType = otDrawPolyline;
		refreshWindow();
		break;
	}
	case ID_2D_POLYGON_OUTLINE:
	{
		setCursor(csCross);
		setRubberMode(rmPolygon);
		g_State.g_OperationType = otDrawPolygonOutline;
		break;
	}
	case ID_2D_POLYGON:
	{
		setCursor(csCross);
		setRubberMode(rmPolygon);
		g_State.g_OperationType = otDrawPolygon;
		break;
	}
	case ID_2D_CIRCLE_OUTLINE:
	{
		setCursor(csHand);
		setRubberMode(rmCircle);
		g_State.g_OperationType = otDrawCircle;
		break;
	}
	case ID_2D_ELLIPSE_OUTLINE:
	{
		setCursor(csCross);
		setRubberMode(rmEllipse);
		g_State.g_OperationType = otDrawEllipse;
		break;
	}
	case ID_SEED_FILL:
	{
		setPenColor(BLACK);
		g_State.g_OperationType = otAreaFill;
		g_State.FillMethod = InternalPointsSeedFill;
		setRubberMode(rmLine);
		setCursor(csArrow);
		break;
	}
	case ID_BOUNDARY_SEED_FILL:
	{
		setPenColor(BLACK);
		g_State.g_OperationType = otAreaFill;
		g_State.FillMethod = BoundarySeedFill;
		setRubberMode(rmLine);
		setCursor(csArrow);
		break;
	}
	case ID_SACNLINE_SEED_FILL:
	{
		setPenColor(BLACK);
		g_State.g_OperationType = otAreaFill;
		g_State.FillMethod = ScanLineSeedFill;
		setRubberMode(rmLine);
		setCursor(csArrow);
		break;
	}
	case ID_2D_DRID_2D_DRAW_TRIANGLE_Outline:
		g_State.g_OperationType = otDrawTriangleOutline;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		break;
	case ID_DRAW_TRIANGLE:
		g_State.g_OperationType = otDrawTriangle;
		setRubberMode(rmPolygon);
		setCursor(csCross);
		break;
	case ID_2D_CLIP_LINE:
		g_State.g_OperationType = otCai;
		setRubberMode(rmRectangle);
		setCursor(csArrow);
		break;
	case ID_2D_LINECLIP_MIDPOINT:
		g_State.g_OperationType = otCai;
		g_State.ClipMethod = MidPoint;
		setRubberMode(rmRectangle);
		setCursor(csArrow);
		break;
	case ID_2D_CLIP_POLYGON:
		g_State.g_OperationType = otCai;
		g_State.ClipMethod = Sutherland_Hodgeman;
		setRubberMode(rmRectangle);
		setCursor(csArrow);
		break;
	case ID_2D_CLIP_ALLPOLYGON:
		g_State.g_OperationType = otWeilerAtherton;
		setRubberMode(rmPolygon);
		setCursor(csArrow);
		break;
	case ID_RESET:
	{
		// ��յ�ǰ����
		Reset = true;
		g_ctx->Reset();
		g_ctx->clerarCanvas();
		Reset = false;
		break;
	}

	case ID_COLOR_RED:
	{
		setPenColor(RED);
		break;
	}
	case ID_COLOR_GREEN:
		setPenColor(GREEN);
		break;
	case ID_COLOR_BLUE:
		setPenColor(BLUE);
		break;
	case ID_2D_PAN:
		setRubberMode(rmNone);
		setCursor(csArrow);
		g_State.g_OperationType = otPan;
		break;
	case ID_ADD_LAYER:
	{
		setCursor(csHand);
		g_State.g_OperationType = otPan;
		setRubberMode(rmNone);
		Layer* pLayer = Utils::openLayer();
		if (pLayer)
		{
			g_ctx->AddLayer(pLayer);
			//��ӵ�һ��ͼ��ʱ���ó�ʼ�任����ͱ�����
			if (g_ctx->GetLayerCount() == 2)
			{
				g_ctx->InitMap();
			}
			refreshWindow();
		}
	}
	break;
	case ID_TRANS_RESET:
		g_ctx->Reset();
		refreshWindow();
		break;
	case ID_3D_CUBE:
		g_State.g_3DMode = Cube;
		g_State.g_3DState = NoneState;
		g_State.g_OperationType = Pan3D;
		g_ctx->g_3DRender->isCheckBack = false;
		g_ctx->clerarCanvas();
		refreshWindow();
		break;
	case ID_3D_Z_BUFFER:
		g_State.g_3DMode = Cube;
		g_State.g_3DState = Blanking;
		g_State.g_OperationType = Pan3D;
		g_ctx->g_3DRender->isCheckBack = true;
		g_ctx->g_3DRender->reset3D();
		g_ctx->clerarCanvas();
		refreshWindow();
		break;
	case ID_3D_CHECKBACK:
		if (g_ctx->g_3DRender->isCheckBack)
		{
			g_ctx->g_3DRender->isCheckBack = false;
		}
		else
		{
			g_ctx->g_3DRender->isCheckBack = true;
		}
		refreshWindow();
		break;
	case ID_3D_TEXTURE:
		g_State.g_3DMode = Cube;
		g_State.g_3DState = Texture;
		g_State.g_OperationType = Pan3D;
		g_ctx->g_3DRender->isCheckBack = false;
		Raster::texture_init();
		g_ctx->clerarCanvas();
		refreshWindow();
		break;
	case ID_3D_LIGHT_SIMPLE:
		g_State.g_3DMode = Cube;
		g_State.g_3DState = LightSimple;
		g_State.g_OperationType = Pan3D;
		g_ctx->g_3DRender->isCheckBack = false;
		refreshWindow();
		break;
	case ID_3D_GOURAND:
		g_State.g_3DMode = Cube;
		g_State.g_3DState = LightGourand;
		g_State.g_OperationType = Pan3D;
		g_ctx->g_3DRender->isCheckBack = false;
		refreshWindow();
		break;
	case IDM_EXIT:
		PostQuitMessage(0);
		break;
	}
	if (action_tool != NULL) delete action_tool;
	action_tool = CreateToolFactory::getTool(g_ctx);
}

///��������Ϣ
void handleKeyMessage(int key)
{
	bool bCtrlPressed = isCtrlKeyPressed();//�Ƿ�ͬʱ������Ctrl��
	bool bShiftPressed = isShiftKeyPressed();//�Ƿ�ͬʱ������Shift��
	double pan = 10.0;
	switch (key)
	{
	case VK_UP: // ��һ�У��Ϲ��
		if (isCtrlKeyPressed())
		{
			if (g_State.g_3DState == LightSimple || g_State.g_3DState == LightGourand)
			{
				g_ctx->g_3DRender->rotateLorE = false;
			}
		}
		else
		{
			if(g_State.g_3DMode == Cube) g_ctx->g_3DRender->rotateLorE = true;
			g_ctx->translate(0, pan);
		}
		if (g_State.g_3DMode == Cube)
		{
			g_ctx->g_3DRender->eye_Rotate_Update();
			g_ctx->g_3DRender->set_eye_Angle(0.04);
		}
		refreshWindow();
		break;
	case VK_DOWN:
		if (isCtrlKeyPressed())
		{
			if (g_State.g_3DState == LightSimple || g_State.g_3DState == LightGourand)
			{
				g_ctx->g_3DRender->rotateLorE = false;
			}
		}
		else
		{
			if(g_State.g_3DMode == Cube) g_ctx->g_3DRender->rotateLorE = true;
			g_ctx->translate(0, -pan);
		}
		if (g_State.g_3DMode == Cube)
		{
			g_ctx->g_3DRender->eye_Rotate_Update();
			g_ctx->g_3DRender->set_eye_Angle(-0.04);
		}
		
		refreshWindow();
		break;
	case VK_LEFT:
		if (isCtrlKeyPressed())
		{
			if (g_State.g_3DState == LightSimple || g_State.g_3DState == LightGourand)
			{
				g_ctx->g_3DRender->rotateLorE = false;
			}
			g_ctx->rotate(10);
		}
		else
		{
			if(g_State.g_3DMode == Cube) g_ctx->g_3DRender->rotateLorE = true;
			g_ctx->translate(-pan, 0);
		}
		if (g_State.g_3DMode == Cube)
		{
			g_ctx->g_3DRender->isLRtoUD = false;
			g_ctx->g_3DRender->Xaxis.set(0, 1, 0);
			g_ctx->g_3DRender->set_eye_Angle(0.04);
			//g_ctx->g_3DRender->Cuberotate -= 0.04;
		}
		refreshWindow();
		break;
	case VK_RIGHT:
		
		if (isCtrlKeyPressed())
		{
			if (g_State.g_3DState == LightSimple || g_State.g_3DState == LightGourand)
			{
				g_ctx->g_3DRender->rotateLorE = false;
			}
			g_ctx->rotate(-10);
		}
		else
		{
			if(g_State.g_3DMode == Cube) g_ctx->g_3DRender->rotateLorE = true;
			g_ctx->translate(pan, 0);
		}
		if (g_State.g_3DMode == Cube)
		{
			g_ctx->g_3DRender->isLRtoUD = false;
			g_ctx->g_3DRender->Xaxis.set(0, 1, 0);
			g_ctx->g_3DRender->set_eye_Angle(-0.04);
			//g_ctx->g_3DRender->Cuberotate += 0.04;
		}
		refreshWindow();
		break;
	case 'X':
	{
		if (g_State.g_3DMode == Cube && g_State.g_3DState == Texture)
		{
			Raster::texture_mode == 1 ? Raster::texture_mode = 0 : Raster::texture_mode += 1;
			Raster::texture_init();
		}
		if (isShiftKeyPressed())
			g_ctx->Shear(-0.1, 0);
		else
		{
			g_State.symmetryMethod = symmetryX;
			g_ctx->symmetry(g_State.symmetryMethod);
		}
		refreshWindow();
		break;
	}
	case 'Y':
		if (isShiftKeyPressed())
			g_ctx->Shear(0, -0.1);
		else
		{
			g_State.symmetryMethod = symmetryY;
			g_ctx->symmetry(g_State.symmetryMethod);
		}
		refreshWindow();
		break;
	case 'S':
		if (isShiftKeyPressed())
			g_ctx->Shear(-0.1, -0.1);
		if (g_State.g_3DMode == Cube)
		{
			g_ctx->g_3DRender->CubeScale += 0.1;
		}
		refreshWindow();
		break;
	case 'D':
		if (g_State.g_3DMode == Cube)
		{
			g_ctx->g_3DRender->CubeScale -= 0.1;
		}
		refreshWindow();
		break;
	case 'N':
		if (g_State.g_3DMode == Cube && g_State.g_3DState == Texture)
		{
			g_ctx->g_3DRender->texture_grid += 1;
		}
		refreshWindow();
		break;
	case 'M':
		if (g_State.g_3DMode == Cube && g_State.g_3DState == Texture)
		{
			g_ctx->g_3DRender->texture_grid <= 1 ? g_ctx->g_3DRender->texture_grid = 1 : g_ctx->g_3DRender->texture_grid -= 1;
		}
		refreshWindow();
		break;
	}
}

///���������Ϣ
void handleMouseMessage(UINT message, int x, int y, int det)
{
	DPtToLPt(x, y, x, y);//�����豸����תΪ�����߼�����
	static bool dragging = false;
	static Point2D pt1;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (g_State.g_3DMode == Cube)
		{
			action_tool->LButtonDown(x, y);
		}
		else
		{
			if (dragging == true) dragging = false;
			else
				if (g_State.g_OperationType == otPan)
				{
					pt1.x = x;
					pt1.y = y;
					dragging = true;
				}
		}
		break;
	case WM_MOUSEMOVE:
		if (g_State.g_3DMode == Cube)
		{
			action_tool->MouseMove(x, y);
		}
		else
		{
			if (dragging)
			{
				if (pt1.x != x || pt1.y != y)
				{
					g_ctx->translate(x - pt1.x, y - pt1.y);
					pt1.x = x;
					pt1.y = y;
					refreshWindow();
				}
			}
		}
		refreshWindow();
		break;
	case WM_LBUTTONUP:
	{
		if (g_State.g_3DMode == Cube)
		{
			action_tool->LButtonUp(x, y);
			return;
		}
		/*PixelPoint pc[2];*/
		//��MyGeometryLibrary.h���createTool���LButtonUp����
		int c = getRubberPointCount();
		rm = getRubberMode();
		if (rm == rmNone || rm == rmPolyline || rm == rmPolygon) break;
		if (c == 2) action_tool->LButtonUp();
	}
	break;
	case WM_RBUTTONDOWN:
	{
		if (g_State.g_3DMode == Cube)
		{
			action_tool->RButtonDown(x,y);
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		//�ù������RButtonUp���������ʼ��
		rm = getRubberMode();
		if (g_State.g_3DMode == Cube)
		{
			action_tool->RButtonUp();
			break;
		}
		if (rm != rmPolyline && rm != rmPolygon) break;
		action_tool->RButtonUp();
		//g_State.g_3DMode == Cube ? action_tool->RButtonDown(x,y) : action_tool->RButtonDown();
		
	}
	break;
	
	case WM_MOUSEWHEEL:
	{
		if (isCtrlKeyPressed())
		{
			if (g_State.g_3DState == LightSimple || g_State.g_3DState == LightGourand)
			{
				g_ctx->g_3DRender->rotateLorE = false;
			}
			x = 0;
			y = 0;
		}
		else
		{
			if(g_State.g_3DState == LightSimple || g_State.g_3DState == LightGourand)
			g_ctx->g_3DRender->rotateLorE = true;
		}
		if (det > 0)
		{
			g_ctx->scale((double)x, (double)y, 0.9, 0.9);
			if(g_State.g_3DMode == Cube) g_ctx->g_3DRender->set_eye_far(0.5);
		}
		else
		{
			g_ctx->scale(x, y, 1.0 / 0.9, 1.0 / 0.9);
			if(g_State.g_3DMode == Cube) g_ctx->g_3DRender->set_eye_far(-0.5);
		}
		
		refreshWindow();
	}
	break;
	}
}

///���������Ϣ
void display()
{
	/*if (g_State.g_3DMode == Cube) return;*/
	int w = getWindowWidth();
	int h = getWindowHeight();

	//setWindowSize(1200, 1200);
	setYUp(true); // y������
	//setOrig(0, h);// ԭ��Ϊ�������½�
	//rm = getRubberMode();
	setOrig(w / 2, h / 2);
	
	g_ctx->Render();
	//setPixel(1, 2, 3, BLACK);
}

///������Ϣ����ڵ�
LRESULT handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND://�˵���Ϣ
	{
		int menuID = LOWORD(wParam);
		handleMenuMessage(menuID);
		return FALSE;
	}
	case WM_PAINT://������Ϣ
		display();
		return FALSE;

	case WM_CREATE:
		init((unsigned)hWnd);
		// ����һ�Ż���
		Initialize();
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_KEYDOWN://������Ϣ
	{
		int key = wParam;//��ð�������
		handleKeyMessage(key);
		return FALSE;
	}
	case WM_MOUSEMOVE://����ƶ���Ϣ
	case WM_LBUTTONDOWN://������������Ϣ
	case WM_LBUTTONUP://������������Ϣ
	case WM_RBUTTONUP://����Ҽ�������Ϣ
	case WM_RBUTTONDOWN://����Ҽ�������Ϣ
	case WM_LBUTTONDBLCLK://���˫����Ϣ
	case WM_MOUSEWHEEL://��������Ϣ
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		int det = message == WM_MOUSEWHEEL ? (short)HIWORD(wParam) : 0;
		handleMouseMessage(message, x, y, det);
		return FALSE;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}