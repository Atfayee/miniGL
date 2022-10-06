#ifndef _MYGEODEFINE_H
#define _MYGEODEFINE_H

#include <vector>
#include <string>
#include <algorithm>
//#include "Vec.h"

using namespace std;

#define PI 3.14159265358979323846

// 2D点
template<typename T>
struct _Point2D
{
	_Point2D() { x = y = 0; }
	_Point2D(T x, T y) { this->x = x, this->y = y; }

	T x, y;
	bool operator==(const _Point2D& pt) const
	{
		return x == pt.x && y == pt.y;
	}
	bool operator!=(const _Point2D& pt) const
	{
		return !(*this == pt);
	}
	bool operator<(const _Point2D& pt) const
	{
		if (*this == pt) return false;
		else
		{
			if (x != pt.x) return x < pt.x;
			else return y < pt.y;
		}
	}

	/*operator PixelPoint()
	{
		PixelPoint pt;
		pt.x = (int)this->x;
		pt.y = (int)this->y;
		return pt;
	}*/
};

//typedef _Point2D<double> Point2D;
typedef _Point2D<double> Point2D;
// 交点对象
struct IntersectionPoint
{
	double x = 0;
	double y = 0;
	bool _entering = false;
	bool _isIntersectionPoint = false;
public:
	IntersectionPoint() {}
	IntersectionPoint(double x, double y) { this->x = x; this->y = y; }
	IntersectionPoint(double x, double y, bool isEnter) { this->x = x; this->y = y; this->_entering = isEnter; this->_isIntersectionPoint = true; }
	bool isEntering() { return _entering; }
	bool isIntersectionPoint() { return _isIntersectionPoint; }
	operator Point2D()
	{
		Point2D pt(this->x, this->y);
		return pt;
	}
};
// 2D包围盒
template<typename T>
struct _Box2D
{
	_Box2D() { valid = false; }
	_Box2D(T xmin, T ymin, T xmax, T ymax)
	{
		setBox(xmin, ymin, xmax, ymax);
	}

	T width() { return _xmax - _xmin; }//包围盒宽度
	T height() { return _ymax - _ymin; }//包围盒高度
	T centerX() { return (_xmin + _xmax) / 2; }//包围盒中心点x坐标
	T centerY() { return (_ymin + _ymax) / 2; }//包围盒中心点y坐标
	T xmin() { return _xmin; }
	T ymin() { return _ymin; }
	T xmax() { return _xmax; }
	T ymax() { return _ymax; }

	void setBox(T xmin, T ymin, T xmax, T ymax)//设置包围盒范围
	{
		this->_xmin = xmin, this->_xmax = xmax, this->_ymin = ymin, this->_ymax = ymax;
		valid = true;
	}

	void setBox(_Box2D& box)
	{
		setBox(box._xmin, box._ymin, box._xmax, box._ymax);
	}

	//根据添加的点扩展包围盒
	void expand(T x, T y)
	{
		if (!valid) {
			_xmin = _xmax = x;
			_ymin = _ymax = y;
			valid = true;
		}
		else
		{
			_xmin = min(_xmin, x);
			_xmax = max(_xmax, x);
			_ymin = min(_ymin, y);
			_ymax = max(_ymax, y);
		}
	}

	//根据添加的包围盒扩展包围盒
	void expand(_Box2D<T>& box) {
		if (box.valid)
		{
			if (!valid)
				setBox(box);
			else
			{
				expand(box.xmin(), box.ymin());
				expand(box.xmax(), box.ymax());
			}
		}
	}

protected:
	T _xmin, _ymin;
	T _xmax, _ymax;
	bool valid;//是否有效
};

typedef _Box2D<double> Box2D;

// 图形对象类型
enum GeomType {
	gtUnkown = 0, gtPoint = 1, gtPolyline = 2,
	gtPolygon = 3, gtCircle, gtEllipse, gtTriangle,
	gtTin,
};

// 图形对象基类
struct Geometry
{
	virtual ~Geometry() {}
	virtual GeomType getGeomType() = 0; // 获取图形对象类
	string label;
	unsigned color;
};
struct Geometry2D :Geometry
{
	virtual Box2D getEnvelop() = 0;//获取图形对象包围盒
};

// 点图形对象
struct PointGeometry :Geometry2D
{
	PointGeometry()
	{
		x = y = 0;
	}
	PointGeometry(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	virtual GeomType getGeomType() { return gtPoint; }
	virtual Box2D getEnvelop() { return Box2D(x, y, x, y); }

	double x, y;
};
// 线图形对象
struct PolylineGeometry :Geometry2D
{
	virtual GeomType getGeomType() { return gtPolyline; }

	//运算符重载，获取第i个点
	Point2D& operator[](int i) { return pts[i]; }

	//添加点
	void addPoint(double x, double y)
	{
		pts.push_back(Point2D(x, y));
		envelop.expand(x, y);
	}

	virtual Box2D getEnvelop() { return envelop; }
	const vector<Point2D>& getPts() const
	{
		return pts;
	}
protected:
	//所有点
	vector<Point2D> pts;
	Box2D envelop;
};

// 多边形对象
struct PolygonGeometry :PolylineGeometry
{
	virtual GeomType getGeomType() { return gtPolygon; }
	void initPts(vector<Point2D>& initPoints)
	{
		pts.assign(initPoints.begin(), initPoints.end());
	}
	bool isClockWiseDirection()
	{
		double sum = 0;
		for (auto it_next = pts.begin(), it = it_next++; it_next != pts.end(); it++, it_next++)
		{
			sum += (it->x * it_next->y - it->y * it_next->x);
		}
		//sum += (pts.front().x * pts.back().y - pts.front().y * pts.back().x);
		sum += (pts.back().x * pts.front().y - pts.back().y * pts.front().x);
		return sum < 0;
	}
	void reverseDirection()
	{
		reverse(pts.begin(), pts.end());
	}
};

struct TriangleGeometry :PolylineGeometry
{
	virtual GeomType getGeomType() { return gtTriangle; }
};
// 圆图形对象
struct CircleGeometry :Geometry2D
{
	CircleGeometry()
	{
		x = y = 0;
		r = 0;
	}
	CircleGeometry(double x, double y, double r)
	{
		this->x = x;
		this->y = y;
		this->r = r;
	}
	CircleGeometry(double x, double y, double r, unsigned color)
	{
		this->x = x;
		this->y = y;
		this->r = r;
		this->color = color;
	}
	/*~CircleGeometry()
	{
		delete this;
	}*/

	virtual GeomType getGeomType() { return gtCircle; }
	virtual Box2D getEnvelop() { return Box2D(x - r, y - r, x + r, y + r); }
	Point2D GetCenter() { return Point2D(x, y); }
	double x, y;
	double r;
};
// 椭圆图形对象
struct EllipseGeometry :Geometry2D
{
	EllipseGeometry()
	{
		x1 = y1 = x2 = y2 = 0;
	}
	EllipseGeometry(int x1, int y1, int x2, int y2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
	EllipseGeometry(int x1, int y1, int x2, int y2, unsigned color)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
		this->color = color;
	}

	virtual GeomType getGeomType() { return gtEllipse; }
	virtual Box2D getEnvelop() { return Box2D(x1, y1, x2, y2); }

	void getCenter(double& x, double& y) { x = (x1 + x2) * 0.5, y = (y1 + y2) * 0.5; }
	double getWidth() { return x2 - x1; }
	double getHeight() { return y2 - y1; }

	double x1, y1;
	double x2, y2;
};

// 图层
struct Layer
{
	Layer()
	{
		geomType = gtUnkown;
	}

	Layer(GeomType geomType)
	{
		this->geomType = geomType;
	}

	Layer(GeomType geomType, string LayerType)
	{
		this->geomType = geomType;
		this->LayerType = LayerType;
	}

	virtual ~Layer()
	{
		for (size_t i = 0, size = geometrySet.size(); i < size; ++i)
			delete geometrySet[i];//析构函数删除所有图形对象
		if (!g_Points.empty())
			g_Points.clear();
	}

	void DeleteGeometry()
	{
		for (size_t i = 0, size = geometrySet.size(); i < size; ++i)
		{
			delete geometrySet[i];//析构函数删除所有图形对象
		}
		geometrySet.clear();
		if (!g_Points.empty())
			g_Points.clear();
	}
	void DeleteGeo(int i)
	{
		delete geometrySet[i];
		geometrySet.erase(geometrySet.begin() + i);
	}

	//运算符重载，返回第i个图形对象
	Geometry2D* operator[](int i) { return geometrySet[i]; }

	//添加图像对象
	void addGeometry(Geometry2D* pGeometry, bool updateEnvelop = false)
	{
		geometrySet.push_back(pGeometry);
		if (updateEnvelop)
			envelop.expand((Box2D&)pGeometry->getEnvelop());
	}

	//设置图层范围
	void setEnvelop(double xmin, double ymin, double xmax, double ymax)
	{
		envelop.setBox(xmin, ymin, xmax, ymax);
	}

	//获取图层包含的图形对象数量
	int getGeometryCount() { return geometrySet.size(); }
	// 监测当前图层数量是否为空
	bool EmptyGeometry()
	{
		if (getGeometryCount())
			return false;
		else
			return true;
	}

	vector<Geometry2D*> geometrySet;//图形对象集合
	Box2D envelop;//图层范围对应的包围盒
	GeomType geomType;//图层类型
	string LayerType = "SimpleGeometry";
	vector<Point2D> g_Points;
};
// 数据集
struct Dataset
{
	virtual ~Dataset()
	{
		for (size_t i = 0, size = layerSet.size(); i < size; ++i)
			delete layerSet[i];//析构函数删除图层
	}

	//运算符重载，返回第i个图层
	Layer* operator[](int i) { return layerSet[i]; }

	int indexOf(Layer* pLayer) {
		for (int i = 0; i < layerSet.size(); ++i)
		{
			if (pLayer = layerSet[i]) return i;
		}
		return -1;
	}
	Layer* getBack()
	{
		return layerSet.back();
	}

	//获取图层数
	int getLayerCount() { return layerSet.size(); }

	//添加图层
	void addLayer(Layer* pLayer)
	{
		layerSet.push_back(pLayer);
	}

	//图层集合
	vector<Layer*> layerSet;
};


// 3D点
//struct Vertex3D
//{
//	vector3D wpos;
//
//	Vertex3D() {};
//	Vertex3D(double x, double y, double z)
//	{
//		wpos.set(x, y, z);
//	}
//
//	
//};
//
//struct Traingle
//{
//	Traingle()
//	{
//		ids[0] = ids[1] = ids[3] = 0;
//	}
//	Traingle(int v0, int v1, int v2)
//	{
//		ids[0] = v0; ids[1] = v1; ids[2] = v2;
//	}
//	int operator[](int i)
//	{
//		return ids[i];
//	}
//	int next(int i)
//	{
//		return (i + 1) % 3;
//	}
//	int ids[3]; // 保存顶点索引
//};
//
//// 三角网
//struct Tin :Geometry
//{
//	vector<Vertex3D> vertexSet;
//	vector<Traingle> faces;
//	vector<vector3D> faceNormals;
//
//	virtual GeomType getGeomType() { return gtTin; }
//	void calcNormals();
//	int getVertexNumber() { return vertexSet.size(); }
//	int getFacesNumber() { return faces.size(); }
//};
//
//struct Box3D
//{
//
//};

#endif // !_MYGEODEFINE_H
