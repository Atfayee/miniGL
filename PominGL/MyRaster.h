#ifndef _MYRASTER_H
#define _MYRASTER_H
#include "Graphic.h"
#include "MyGeoDefine.h"
#include "Vec.h"
#include "FractalImageGenerator.h"

class Raster
{
public:
	Raster()
	{
		Raster::texture_init();
	}
	static int texture_w; // ÎÆÀí¿í
	static int texture_h; // ÎÆÀí¸ß
	static int texture_mode;

	static unsigned* pImage;
	static unsigned** pImageHeader;
	static unsigned* pHeader;
	static unsigned texture[256][256];
	static void drawPoint(int x, int y, unsigned color);
	static void drawCell(int row, int col, unsigned color);
	static void drawGrid();
	static void drawLine(double x0, double y0, double x1, double y1, unsigned color);
	static void drawPolyline(PixelPoint* data, int size, unsigned color);
	static void drawPolygon(PixelPoint* data, int size, unsigned color);
	static void drawPolygonOutline();
	static void drawRectangle();
	static void drawRectangleOutline();
	static void drawCircle(double x, double y, double r, unsigned color);
	static void drawCircleOutline(double x, double y, double r, unsigned color);
	static void drawEllipse(double xCenter, double yCenter, double width, double height, unsigned color);

	static void drawTriangle(PixelPoint* pts, unsigned color);
	static void ChangeXYToGrid(int& x0, int& y0, int& x1, int& y1);
	static void ChangeOf_XYs(PixelPoint* Points, int Count);
	static void CohenSutherland(PixelPoint pt1, PixelPoint pt2, Layer* container);
	static void MidPointClipAlgorithm(PixelPoint pt1, PixelPoint pt2, Layer* container);
	static void Sutherland_Hodgeman(PixelPoint rectPt1, PixelPoint rectPt2, Layer* container);
	static vector<PolygonGeometry*> WeilerAtherton(PolygonGeometry* subject, PolygonGeometry* clipping);
	static void Draw3DTriangle(vector3D& p1, vector3D& p2, vector3D& p3, Color color);
	static void Draw3DTriangle(texture_v& p1, texture_v& p2, texture_v& p3);;
	static void texture_init();
	static unsigned read_Color(double u, double v);
};

double ToGetDeepthZ(vector3D pt0, vector3D pt1, vector3D pt2, int x, int y);

#endif // !_MYRASTER_H
