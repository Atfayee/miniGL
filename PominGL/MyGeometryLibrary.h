#ifndef _MYGEOMETRYLIBRARY_H
#define _MYGEOMETRYLIBRARY_H

#include "MyGeoDefine.h"
#include "Graphic.h"
#include "Canvas.h"
struct PixelPoint;

//TODO 也可以移至工具类里面
class GeometryLibrary
{
public:
	static Geometry2D* createPointGeometry(double x, double y);
	static Geometry2D* createPolylineGeometry(PixelPoint* pts, int size, unsigned color);
	static Geometry2D* createPolylineGeometry(Point2D* pts, int size);
	static Geometry2D* createPolylineGeometry(PixelPoint& pt1, PixelPoint& pt2, unsigned color);
	static Geometry2D* createPolygonGeometry(PixelPoint* pts, int size);
	static Geometry2D* createPolygonOutlineGeometry(PixelPoint* pts, int size, unsigned color);
	static Geometry2D* createPolygonGeometry(Point2D* pts, int size);
	static Geometry2D* createPolygonOutlineGeometry(Point2D* pts, int size);
	static Geometry2D* createCircleGeometry(double x, double y, double r);
	static Geometry2D* createCircleGeometry(double x1, double y1, double x2, double y2);
	static Geometry2D* createCircleGeometry(double x1, double y1, double x2, double y2, unsigned color);
	static Geometry2D* createEllipseGeometry(double x1, double y1, double x2, double y2);
	static Geometry2D* createEllipseGeometry(double x1, double y1, double x2, double y2, unsigned color);
	static Geometry2D* creatRectangleGeometry(double x1, double y1, double x2, double y2);
	static Geometry2D* creatRectangleOutlineGeometry(double x1, double y1, double x2, double y2);
	static Geometry2D* createTriangleGemoetry(PixelPoint* pts, int size);
};
class Tool
{
protected:
	Canvas* g;
public:
	Tool(Canvas* g)
	{
		this->g = g;
	}
	virtual void LButtonUp() {};
	virtual void LButtonUp(int x, int y) {};
	virtual void LButtonDown(int x, int y) {};
	virtual void RButtonUp() {};
	virtual void RButtonUp(int x, int y) {};
	virtual void RButtonDown() {};
	virtual void RButtonDown(int x, int y) {};
	virtual void MouseMove(int x, int y) {};
	//virtual void draw();
};
class NoneTool : public Tool
{
public:
	NoneTool(Canvas* g) :Tool(g) {}
	void LButtonUp() {}
	void RButtonUp() {}
};
class CreateLine : public Tool
{
protected:
	PixelPoint pt1, pt2;
public:
	CreateLine(Canvas* g) : Tool(g) { }
	void LButtonUp()
	{
		getRubberPoints(pt1, pt2);
		if (pt1.x == pt2.x && pt1.y == pt2.y) return;
		g->AddGeometry(GeometryLibrary::createPolylineGeometry(pt1, pt2, getPenColor()));
		refreshWindow();
	}
	void RButtonUp()
	{
	}
};

class CreateCircle : public CreateLine
{
public:
	CreateCircle(Canvas* g) :CreateLine(g) { }
	void LButtonUp()
	{
		getRubberPoints(pt1, pt2);
		if (pt1.x == pt2.x && pt1.y == pt2.y) return;
		g->AddGeometry(GeometryLibrary::createCircleGeometry(pt1.x, pt1.y, pt2.x, pt2.y, getPenColor()));
		refreshWindow();
	}
	void RButtonUp()
	{
	}
};

class CreateEllipse : public CreateLine
{
protected:
	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
public:
	CreateEllipse(Canvas* g) : CreateLine(g) {}
	void LButtonUp()
	{
		getRubberPoints(pt1, pt2);
		double x1 = pt1.x;
		double y1 = pt1.y;
		double x2 = pt2.x;
		double y2 = pt2.y;
		if (x1 == x2 || y1 == y2) return;
		g->AddGeometry(GeometryLibrary::createEllipseGeometry(x1, y1, x2, y2, getPenColor()));
		refreshWindow();
	}
	void RButtonUp()
	{
	}
};

class CreateRectangle : public CreateEllipse
{
public:
	CreateRectangle(Canvas* g) : CreateEllipse(g) {}
	void LButtonUp()
	{
		getRubberPoints(pt1, pt2);
		double x1 = pt1.x;
		double y1 = pt1.y;
		double x2 = pt2.x;
		double y2 = pt2.y;
		if (x1 == x2 || y1 == y2) return;
		g->AddGeometry(GeometryLibrary::creatRectangleGeometry(x1, y1, x2, y2));
		refreshWindow();
	}
	void RButtonUp()
	{
	}
};

class CreateRectangleOutline : public CreateEllipse
{
public:
	CreateRectangleOutline(Canvas* g) : CreateEllipse(g) {}
	void LButtonUp()
	{
		getRubberPoints(pt1, pt2);
		double x1 = pt1.x;
		double y1 = pt1.y;
		double x2 = pt2.x;
		double y2 = pt2.y;
		if (x1 == x2 || y1 == y2) return;
		g->AddGeometry(GeometryLibrary::creatRectangleOutlineGeometry(x1, y1, x2, y2));
		refreshWindow();
	}
	void RButtonUp()
	{
	}
};

class CreatePolyline : public Tool
{
protected:
	int c = 0;
public:
	CreatePolyline(Canvas* g) :Tool(g) {}
	void LButtonUp() {}
	void RButtonUp()
	{
		//TODO 具体实现可以放到cpp文件里
		c = getRubberPointCount();
		if (c < 2) return;
		vector<PixelPoint> pts(c);
		getRubberPoints(pts.data());
		g->AddGeometry(GeometryLibrary::createPolylineGeometry(pts.data(), pts.size(), getPenColor()));
		refreshWindow();
	}
};

class CreatePolygonOutline : public CreatePolyline
{
public:
	CreatePolygonOutline(Canvas* g) :CreatePolyline(g) {}
	void LButtonUp() {}
	void RButtonUp()
	{
		c = getRubberPointCount();
		if (c < 2) return;
		vector<PixelPoint> pts(c);
		getRubberPoints(pts.data());
		g->AddGeometry(GeometryLibrary::createPolygonOutlineGeometry(pts.data(), pts.size(), getPenColor()));
		refreshWindow();
	}
};

class CreatePolygon : public CreatePolyline
{
public:
	CreatePolygon(Canvas* g) :CreatePolyline(g) {}
	void LButtonUp() {}
	void RButtonUp()
	{
		c = getRubberPointCount();
		if (c < 2) return;
		vector<PixelPoint> pts(c);
		getRubberPoints(pts.data());
		g->AddGeometry(GeometryLibrary::createPolygonGeometry(pts.data(), pts.size()));
		refreshWindow();
	}
};

class CreateTraingle : public CreatePolyline
{
public:
	CreateTraingle(Canvas* g) : CreatePolyline(g) {}
	void RButtonUp()
	{
		c = getRubberPointCount();
		if (c < 3) return;
		vector<PixelPoint> pts(c);
		getRubberPoints(pts.data());
		g->AddGeometry(GeometryLibrary::createTriangleGemoetry(pts.data(), 3));
		refreshWindow();
	}
};

class CreateTraingleOutline : public CreatePolyline
{
public:
	CreateTraingleOutline(Canvas* g) : CreatePolyline(g) {}
	void RButtonUp()
	{
		c = getRubberPointCount();
		if (c < 3) return;
		vector<PixelPoint> pts(c);
		getRubberPoints(pts.data());
		g->AddGeometry(GeometryLibrary::createPolygonOutlineGeometry(pts.data(), 3, getPenColor()));
		refreshWindow();
	}
};

class FillTool : public Tool
{
private:
	PixelPoint p1[2];
public:
	FillTool(Canvas* g) :Tool(g) {}
	void LButtonUp()
	{
		getRubberPoints(p1);
		g->AddFillPoint(p1[0]);
		refreshWindow();
	}
};

class WeilerAthertonTailor : public CreatePolygon
{
public:
	WeilerAthertonTailor(Canvas* g) : CreatePolygon(g) {}
	void RButtonUp()
	{
		c = getRubberPointCount();
		if (c < 2) return;
		vector<PixelPoint> pts(c);
		getRubberPoints(pts.data());
		PolygonGeometry* polygon = (PolygonGeometry*)GeometryLibrary::createPolygonGeometry(pts.data(), pts.size());
		g->WeilerAthertonClip(polygon);
		refreshWindow();
	}
};

class ClipTool : public Tool
{
public:
	ClipTool(Canvas* g) :Tool(g) {};
	void LButtonUp();
};

class Drag3DTool : public Tool
{
private:
	int lbuttondown[2];
	int mousemove[2];
	bool drag = false;
public:
	Drag3DTool(Canvas* g) :Tool(g) {};
	void LButtonDown(int x, int y);
	void MouseMove(int x, int y);
	void LButtonUp();
};

class Pan3DTool : public Tool
{
private:
	Point2D leftmousedown;
	Point2D leftmouseup;
	Point2D rightmousedown;
	Point2D rightmouseup;
	Point2D mousemove;
	Point2D endPt;
	double screen_width;
	double screen_height;
	bool drag;
	bool rotate;
public:
	Pan3DTool(Canvas* g) :Tool(g)
	{
		drag = false;
		rotate = false;
		screen_height = getWindowHeight();
		screen_width = getWindowWidth();
	}
	void LButtonDown(int x, int y);
	void LButtonUp() {};
	void LButtonUp(int x, int y);

	void RButtonDown(int x, int y);
	void RButtonUp();
	void MouseMove(int x, int y);
	void RotateSphere(double sX,double sY,double eX,double eY);
};

#endif // !_MYGEOMETRY_H