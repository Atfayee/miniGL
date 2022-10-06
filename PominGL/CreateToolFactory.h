#ifndef _CREATETOOLFACTORY_H
#define _CREATETOOLFACTORY_H
#include <map>
#include <functional>
#include "DrawState.h"
#include "Canvas.h"
#include "MyGeometryLibrary.h"

using namespace std;
class CreateToolFactory
{
public:
	static Tool* getTool(Canvas* g_Canvas)
	{
		switch (g_State.g_OperationType)
		{
		case otDrawPolyline:
			return new CreatePolyline(g_Canvas);
			break;
		case otDrawPolygonOutline:
			return new CreatePolygonOutline(g_Canvas);
			break;
		case otDrawPolygon:
			return new CreatePolygon(g_Canvas);
			break;
		case otDrawLine:
			return new CreateLine(g_Canvas);
			break;
		case otDrawCircle:
			return new CreateCircle(g_Canvas);
			break;
		case otDrawEllipse:
			return new CreateEllipse(g_Canvas);
			break;
		case otDrawRectangle:
			return new CreateRectangle(g_Canvas);
			break;
		case otDrawRectangleOutline:
			return new CreateRectangleOutline(g_Canvas);
			break;
		case otDrawTriangle:
			return new CreateTraingle(g_Canvas);
			break;
		case otDrawTriangleOutline:
			return new CreateTraingleOutline(g_Canvas);
			break;
		case otAreaFill:
			return new FillTool(g_Canvas);
			break;
		case otWeilerAtherton:
			return new WeilerAthertonTailor(g_Canvas);
			break;
		case otCai:
			return new ClipTool(g_Canvas);
			break;
		case Pan3D:
			return new Pan3DTool(g_Canvas);
			break;
		default:
			return new NoneTool(g_Canvas);
		}
	}
};
//const map < OperationType, std::function<Tool*(Canvas*)> > CreateToolFactory = {
//	{otDrawPolyline,[](Canvas* g_Canvas) {return new CreatePolyline(g_Canvas); }},
//};

#endif
