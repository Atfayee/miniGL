#pragma once
#include "MyGeoDefine.h"
#include "Graphic.h"

// 二维坐标的齐次坐标表示
class vector2D
{
public:
	double x, y, h;
	vector2D()
	{
		x = y = h = 0;
	}
	vector2D(const Point2D& pt)
	{
		this->x = pt.x;
		this->y = pt.y;
		this->h = 1;
	}
	// 重载类型转换函数
	operator PixelPoint()
	{
		PixelPoint pt;
		pt.x = (int)x, pt.y = (int)y;
		return pt;
	}
	operator Point2D()
	{
		Point2D pt;
		pt.x = x;
		pt.y = y;
		return pt;
	}
};

// 三维坐标的齐次坐标表示
class vector3D
{
public:
	double x, y, z, h;
	vector3D()
	{
		x = y = z = 0;
		h = 1;
	}
	~vector3D(){}

	vector3D(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->h = 1;
	}

	vector3D(PixelPoint& pt)
	{
		this->x = pt.x;
		this->y = pt.y;
		this->z = pt.z;
		this->h = 1;
	}
	void set(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->h = 1;
	}
	operator PixelPoint()
	{
		PixelPoint pt;
		pt.x = (int)this->x;
		pt.y = (int)this->y;
		pt.z = this->z;
		return pt;
	}
	vector3D operator+(const vector3D& pt)
	{
		vector3D NewVec3D;
		NewVec3D.x = x + pt.x;
		NewVec3D.y = y + pt.y;
		NewVec3D.z = z + pt.z;
		return NewVec3D;
	}
	vector3D operator-(const vector3D& pt)
	{
		vector3D NewVec3D;
		NewVec3D.x = x - pt.x;
		NewVec3D.y = y - pt.y;
		NewVec3D.z = z - pt.z;
		return NewVec3D;
	}
	// 乘
	vector3D operator*(const vector3D& pt)
	{
		vector3D NewVec3D;
		NewVec3D.x = x * pt.x;
		NewVec3D.y = y * pt.y;
		NewVec3D.z = z * pt.z;
		return NewVec3D;
	}
	vector3D operator/(const double& f)
	{
		vector3D NewVec3D;
		NewVec3D.x = x / f;
		NewVec3D.y = y / f;
		NewVec3D.z = z / f;
		return NewVec3D;
	}
	vector3D operator*(const double& fv)
	{
		vector3D NewVec3D;
		NewVec3D.x = x * fv;
		NewVec3D.y = y * fv;
		NewVec3D.z = z * fv;
		return NewVec3D;
	}
	vector3D operator+=(const vector3D& pt)
	{
		*this = *this + pt;
		return *this;
	}
	vector3D operator-=(const vector3D& pt)
	{
		*this = *this - pt;
		return *this;
	}
	vector3D operator*=(const vector3D& pt)
	{
		*this = *this * pt;
		return *this;
	}
	
	// 获取向量长度
	double getVectorLength()
	{
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	// 向量点乘
	double vector_dotMutiply(vector3D& pt)
	{
		return this->x * pt.x + this->y * pt.y + this->z * pt.z;
	}

	// 向量叉乘
	vector3D vector_crossMutiply(vector3D& pt)
	{
		vector3D result;
		result.x = this->y * pt.z - this->z * pt.y;
		result.y = this->z * pt.x - this->x * pt.z;
		result.z = this->x * pt.y - this->y * pt.x;
		return result;
	}

	// 归一化
	void vector_normalize()
	{
		double len = this->getVectorLength();
		if (len != 0)
		{
			double temp = 1.0 / len;
			this->x = this->x * temp;
			this->y = this->y * temp;
			this->z = this->z * temp;
		}
	}

};

struct rgbs
{
	double r, g, b;
	rgbs(double r, double g, double b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
	rgbs()
	{
		r = g = b = 0;
	}
};
/// <summary>
/// 带纹理坐标的向量
/// </summary>
class texture_v
{
public:
	vector3D pos;
	Point2D tex;
	double Light;
	rgbs RGB;

	texture_v(vector3D& pos, Point2D& tex)
	{
		this->pos = pos;
		this->tex = tex;
		this->Light = 0;
	}
	texture_v(vector3D& pos, double u,double v)
	{
		this->pos = pos;
		this->tex.x = u; this->tex.y = v;
		this->Light = 0;
	}
	texture_v(vector3D& pos, double u, double v, double r, double g, double b, double alph)
	{
		this->pos = pos;
		this->tex.x = u; this->tex.y = v;
		this->RGB.b = b; this->RGB.g = g; this->RGB.r = r;
		this->Light = alph;
	}

	texture_v() {};

	texture_v operator+(const texture_v& pt)
	{
		texture_v NewVec3D;
		NewVec3D.pos.x = pos.x + pt.pos.x;
		NewVec3D.pos.y = pos.y + pt.pos.y;
		NewVec3D.pos.z = pos.z + pt.pos.z;
		NewVec3D.pos.h = pos.h + pt.pos.h;
		NewVec3D.tex.x = tex.x + pt.tex.x;
		NewVec3D.tex.y = tex.y + pt.tex.y;
		NewVec3D.RGB.r = RGB.r + pt.RGB.r;
		NewVec3D.RGB.g = RGB.g + pt.RGB.g;
		NewVec3D.RGB.b = RGB.b + pt.RGB.b;
		NewVec3D.Light = Light + pt.Light;
		return NewVec3D;
	}
	texture_v operator+=(const texture_v& pt)
	{
		*this = *this + pt;
		return *this;
	}

	
};
