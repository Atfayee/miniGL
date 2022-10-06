#include "MyRaster.h"
#include "Graphic.h"
#include "DrawState.h"
#include "MyGraphic.h"
#include <algorithm>
#include <vector>
#include <set>
//using namespace State;
using namespace std;
// 像素坐标转换
void Raster::ChangeXYToGrid(int& x0, int& y0, int& x1, int& y1)
{
	x0 /= g_State.GridWidth; y0 /= g_State.GridHeight;
	x1 /= g_State.GridWidth; y1 /= g_State.GridHeight;
}
// 点集像素坐标转换
void Raster::ChangeOf_XYs(PixelPoint* Points, int Count)
{
	for (int i = 0; i < Count; i++)
	{
		Points[i].x /= g_State.GridWidth;
		Points[i].y /= g_State.GridHeight;
	}
}
void Raster::drawPoint(int x, int y, unsigned color)
{
	setPixel(x, y, color);
}
// 绘制网格像素
void Raster::drawCell(int col, int row, unsigned color)
{
	for (int i = row * g_State.GridHeight; i < (row + 1) * g_State.GridHeight; ++i)
		for (int j = col * g_State.GridWidth; j < (col + 1) * g_State.GridWidth; ++j)
			setPixel(j, i, color);
}

// 绘制网格背景图
void Raster::drawGrid()
{
	int orig_x, orig_y;
	getOrig(orig_x, orig_y);

	int left = (int)((float)orig_x / g_State.GridWidth + 0.5);
	int right = (int)((float)(getWindowWidth() - orig_x) / g_State.GridWidth + 0.5);
	int top = (int)((float)orig_y / g_State.GridHeight + 0.5);
	int bottom = (int)((float)(getWindowHeight() - orig_y) / g_State.GridHeight + 0.5);

	int ymin, ymax;
	if (isYUp())
	{
		ymin = -bottom;
		ymax = top;
	}
	else
	{
		ymin = -top;
		ymax = bottom;
	}
	for (int col = -left; col <= right; ++col)
		for (int row = ymin; row <= ymax; ++row)
		{
			drawCell(col, row, (row + col) % 2 ? g_State.GridColor1 : g_State.GridColor2);
		}
}

void Raster::drawLine(double x0, double y0, double x1, double y1, unsigned color)
{
	if (g_State.DrawLineMethod == dlDDA)
		DDALine(x0, y0, x1, y1, color);
	else
		MidPointLine(x0, y0, x1, y1, color);
}

void Raster::drawPolyline(PixelPoint* data, int size, unsigned color)
{
	//drawPolyline(实现）
	for (int i = 0, c = size; i < c - 1; ++i)
	{
		Raster::drawLine(data[i].x, data[i].y, data[(i + 1)].x, data[(i + 1)].y, color);
	}
}

void Raster::drawPolygon(PixelPoint* data, int size, unsigned color)
{
	//直接填充水平线
	HorizonEdgeFill(data, size, color);
	int ymin = 0;
	int ymax = 0;
	GetYMinMax(data, size, ymin, ymax); // 获取坐标点最大最小y值
	vector<State::tagEDGE> aetEDGE; // 活性边表AET
	vector<vector<State::tagEDGE>> etEDGE(ymax - ymin + 1); // 边表
	InitET(etEDGE, data, size, ymin, ymax); // 初始化边表

	ScanLineFill(aetEDGE, etEDGE, ymin, ymax, color); // 进行扫描
}

void Raster::drawPolygonOutline()
{
	
}

//void Raster::drawRectangle()
//{
//	//TODO drawRectangle(未实现)
//}
//
//void Raster::drawRectangleOutline()
//{
//	//TODO drawRectangleOutline(未实现)
//}

void Raster::drawCircle(double x, double y, double r, unsigned color)
{
	// MidBresenham algorithm
	double x0, y0, d;
	x0 = 0;
	y0 = r;
	d = 1 - r;
	CirclePlot(round(x), round(y), round(x0), round(y0), color);
	while (x0 <= y0)
	{
		if (d < 0)
		{
			d += 2 * x0 + 3;
		}
		else
		{
			d += 2 * (x0 - y0) + 5;
			y0--;
		}
		x0++;
		CirclePlot(round(x), round(y), round(x0), round(y0), color);
	}
}

void Raster::drawCircleOutline(double x, double y, double r, unsigned color)
{
	
}

void Raster::drawEllipse(double xCenter, double yCenter, double width, double height, unsigned color)
{
	double Rx = width / 2, Ry = height / 2;
	double a = Rx, b = Ry;
	double x0 = xCenter, y0 = yCenter;
	double Rx2 = a * a, Ry2 = b * b;   // 椭圆半径的平方
	double x = 0, y = b;          // 相对中心偏移量x,y
	double d;  // 中点值
	EllipsePlot(x0, y0, x, y, color);
	/*区域1*/
	d = Ry2 - (Rx2 * Ry) + (0.25 * Rx2);
	while (Ry2 * (x + 1) < Rx2 * (y - 0.5))
	{
		if (d < 0) d += Ry2 * (2 * x + 3);
		else
		{
			d += Ry2 * (2 * x + 3) + Rx2 * (-2 * y + 2);
			y--;
		}
		x++;
		EllipsePlot(x0, y0, x, y, color);
	}
	/*区域2*/
	d = Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2;
	while (y > 0)
	{
		if (d < 0) {
			d += Ry2 * (2 * x + 2) + Rx2 * (-2 * y + 3);
			x++;
		}
		else d += Rx2 * (-2 * y + 3);
		y--;
		EllipsePlot(x0, y0, x, y, color);
	}
}

//运算重载，定义点的运算
PixelPoint operator-(const PixelPoint& a, const PixelPoint& b) {
	PixelPoint c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}
PixelPoint operator*(const PixelPoint& a, const float& b) {
	PixelPoint c;
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
	return c;
}
PixelPoint operator+(const PixelPoint& a, const PixelPoint& b) {
	PixelPoint c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}
//画实心三角形
void Raster::drawTriangle(PixelPoint* pts, unsigned color) {
	PixelPoint t0 = pts[0];
	PixelPoint t1 = pts[1];
	PixelPoint t2 = pts[2];

	if (t0.y == t1.y && t0.y == t2.y) return;

	//t0,t1,t2 由高到低为三角形的三个顶点
	if (t0.y > t1.y)std::swap(t0, t1);
	if (t0.y > t2.y)std::swap(t0, t2);
	if (t1.y > t2.y)std::swap(t1, t2);

	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		//second_half代表三角形一分为二的上和下，1为上半部分，0为下半部分
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		//segment_height为过t1的水平线与t2和t0的距离，即为两个小三角形的高
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		//α为t0和t2与水平线的夹角，β为t2和t1与水平线的夹角
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
		//A为t0和t2的连线，B为t0和t1的连线
		PixelPoint A = t0 + (t2 - t0) * alpha;
		PixelPoint B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			g_State.drawPixelCB(j, t0.y + i, BLACK);
		}
	}
}
struct Codepoint {
	int x, y;
	char code[4];
	//构造函数
	Codepoint() {}
	//构造函数重载
	Codepoint(PixelPoint pt) {
		this->x = pt.x;
		this->y = pt.y;
	}
};
//划分矩形区域并对各区域进行编码
// 0101 | 0100 | 0110
// ——————————
// 0001 | 0000 | 0010
// ——————————
// 1001 | 1000 | 1010
//底上右左
Codepoint setCode(Codepoint pt, int x_max, int x_min, int y_max, int y_min) {
	Codepoint temp_pt;
	if (pt.y < y_min)
		temp_pt.code[0] = '1';
	else
		temp_pt.code[0] = '0';
	if (pt.y > y_max)
		temp_pt.code[1] = '1';
	else
		temp_pt.code[1] = '0';
	if (pt.x > x_max)
		temp_pt.code[2] = '1';
	else
		temp_pt.code[2] = '0';
	if (pt.x < x_min)
		temp_pt.code[3] = '1';
	else
		temp_pt.code[3] = '0';
	temp_pt.x = pt.x;
	temp_pt.y = pt.y;
	return temp_pt;
}
//判断直线是否可见（需要裁剪）
//0保留，1舍弃，2裁剪
//flag表示编码code[]里是否有1
int DisplayLine(Codepoint pt1, Codepoint pt2) {
	int flag = 0, i;
	for (i = 0; i < 3; i++) {
		//判断code里是否有1
		if (pt1.code[i] == '1' || pt2.code[i] == '1')
			flag = 1;
	}

	//通过flag对3种情况进行判断
	if (flag == 0)
		//flag==0，code里不含1，则直线在0000内部，全部保留；
		return 0;
	else {
		for (i = 0; i < 3; i++) {
			//有相同编码位为1，则不经过0000，舍弃；
			if ((pt1.code[i] == pt2.code[i]) && pt1.code[i] == '1')
				return 1;
		}
		//code里有1，但经过0000，需要裁剪。
		return 2;
	}
}
//交点代替端点
//总是将pt1设置为直线与0000的交点
PixelPoint SetEndpt(Codepoint pt1, Codepoint pt2, int x_max, int x_min, int y_max, int y_min) {
	PixelPoint pt;
	Codepoint temp;//直线与0000的交点
	int x, y;//x为直线与最近的在0000边界线在x方向上的坐标，y为y方向上的
	float k, m;//k为直线斜率，m为直线与0000交点的坐标
	//裁剪左右两边的线段
	if (pt1.code[3] == '1')
		x = x_min;//pt1在左边
	if (pt1.code[2] == '1')
		x = x_max;//pt1在右边
	if (pt1.code[3] == '1' || pt1.code[2] == '1') {
		//pt1不在0000内
		k = (float)(pt1.y - pt2.y) / (pt1.x - pt2.x);
		m = (float)pt1.y + k * (x - pt1.x);
		temp.x = x;
		temp.y = m;
		if (temp.y<y_max && temp.y>y_min) {
			//新点的y坐标正好在0000内，裁剪结束
			pt.x = temp.x;
			pt.y = temp.y;
			return pt;
		}
	}
	//裁剪上下两边的线段
	if (pt1.code[0] == '1')
		y = y_min;//pt1在底部
	if (pt1.code[1] == '1')
		y = y_max;//pt1在上部
	if (pt1.code[0] == '1' || pt1.code[1] == '1') {
		k = (float)(pt1.y - pt2.y) / (pt1.x - pt2.x);
		m = (float)pt1.x + (y - pt1.y) / k;
		temp.x = m;
		temp.y = y;
		for (int i = 0; i < 3; i++) {
			temp.code[i] = pt1.code[i];
		}
		//再裁剪一次
		pt.x = temp.x;
		pt.y = temp.y;
		return pt;
	}
	else
	{
		//pt1在0000内部，直接返回
		pt.x = pt1.x;
		pt.y = pt1.y;
		return pt;
	}
}
void Raster::CohenSutherland(PixelPoint pt1, PixelPoint pt2, Layer* container) {
	//确定区域0000范围
	int x_max = pt1.x > pt2.x ? pt1.x : pt2.x;
	int x_min = pt1.x < pt2.x ? pt1.x : pt2.x;
	int y_max = pt1.y > pt2.y ? pt1.y : pt2.y;
	int y_min = pt1.y < pt2.y ? pt1.y : pt2.y;
	vector<Geometry2D*> TailorGeo;
	int i = 0;
	while (i < container->getGeometryCount()) {
		//进行循环，确定每一条线，并进行编码
		Geometry2D* pSymbolGeometry = (*container)[i];
		if (pSymbolGeometry->getGeomType() != gtPolyline)
		{
			i++;
			continue;
		}
		PixelPoint Lpt1, Lpt2;
		PolylineGeometry* pPolylineGeometry = (PolylineGeometry*)pSymbolGeometry;
		const vector<Point2D>& pts = pPolylineGeometry->getPts();
		PolylineGeometry* nPolylineGeometry = new PolylineGeometry();
		for (int i = 0; i < (pts.size()-1); i++)
		{
			//得到线的两个端点坐标
			Lpt1.x = pts[i].x;
			Lpt1.y = pts[i].y;
			Lpt2.x = pts[i+1].x;
			Lpt2.y = pts[i+1].y;

			PixelPoint pt3, pt4;
			Codepoint pt1, pt2;
			pt1 = setCode(Codepoint(Lpt1), x_max, x_min, y_max, y_min);
			pt2 = setCode(Codepoint(Lpt2), x_max, x_min, y_max, y_min);
			int display_result = DisplayLine(pt1, pt2);
			switch (display_result)
			{
			case 0://保留
				nPolylineGeometry->addPoint(Lpt1.x, Lpt1.y);
				nPolylineGeometry->addPoint(Lpt2.x, Lpt2.y);
				break;
			case 1://舍弃
				break;
			case 2://裁剪
				pt3 = SetEndpt(pt1, pt2, x_max, x_min, y_max, y_min);
				pt4 = SetEndpt(pt2, pt1, x_max, x_min, y_max, y_min);
				nPolylineGeometry->addPoint(pt3.x, pt3.y);
				nPolylineGeometry->addPoint(pt4.x, pt4.y);
				break;
			}
		}
		if (nPolylineGeometry->getPts().size()) TailorGeo.push_back(nPolylineGeometry);
		container->DeleteGeo(i);

	}
	for (int i = 0; i < TailorGeo.size(); i++)
	{
		container->addGeometry(TailorGeo[i]);
	}
	TailorGeo.clear();
}


//判断线段端点在裁剪端口矩形的里面还是外面
int LptOUTERrect(PixelPoint Lpt, PixelPoint rect[2]) {
	//确定范围
	int x_min, x_max, y_min, y_max;
	x_min = rect[0].x < rect[1].x ? rect[0].x : rect[1].x;
	x_max = rect[0].x > rect[1].x ? rect[0].x : rect[1].x;
	y_min = rect[0].y < rect[1].y ? rect[0].y : rect[1].y;
	y_max = rect[0].y > rect[1].y ? rect[0].y : rect[1].y;
	if (Lpt.x > x_min && Lpt.x<x_max && Lpt.y>y_min && Lpt.y < y_max)
		return 0;
	return 1;
}
//二分逼近法查找确定直线段和窗口的交点
PixelPoint BSearch_findIntersection(PixelPoint outsidePt, PixelPoint insidePt, PixelPoint* rect) {

	PixelPoint mid; //定义中点坐标
	PixelPoint pt1, pt2;
	pt1 = insidePt;
	pt2 = outsidePt;
	mid.x = (insidePt.x + outsidePt.x) / 2;
	mid.y = (insidePt.y + outsidePt.y) / 2;
	int mid_outside = LptOUTERrect(mid, rect);
	while (abs(pt2.x - pt1.x) > 2 || abs(pt2.y - pt1.y) > 2) {
		if (mid_outside) {
			pt2 = mid;
		}
		else {
			pt1 = mid;
		}
		mid.x = (pt1.x + pt2.x) / 2;
		mid.y = (pt1.y + pt2.y) / 2;
		mid_outside = LptOUTERrect(mid, rect);
	}
	return pt1;
}
//判断直线与对角线是否平行，若平行则返回1，无交点
//向量法，叉乘，(x1,y1)×(x2,y2)=x1*y2-x2*y1>0，则在顺时针方向；<0，则在逆时针方向
int judgeParallel(PixelPoint Lpt1, PixelPoint Lpt2, PixelPoint DiagonalPt1, PixelPoint DiagonalPt2) {
	int flag1, flag2;
	//先判断线段两个端点在对角线的异侧
	int d1 = DiagonalPt1.x * Lpt1.y - DiagonalPt1.y * Lpt1.x;
	int d2 = DiagonalPt1.x * Lpt2.y - DiagonalPt1.y * Lpt2.x;
	if (d1 * d2 <= 0) {
		//不平行，相交
		flag1 = 0;
	}
	flag1 = 1;
	//再判断对角线两个端点在线段的异侧
	int d3 = Lpt1.x * DiagonalPt1.y - Lpt1.y * DiagonalPt1.x;
	int d4 = Lpt1.x * DiagonalPt2.y - Lpt1.y * DiagonalPt2.x;
	if (d3 * d4 <= 0) {
		flag2 = 0;
	}
	flag2 = 1;
	if (flag1 == 0 && flag2 == 0)
		return 0;//不平行，相交，返回0
	else
		return 1;
}
//求线段与对角线的交点
PixelPoint getPoint(PixelPoint out1, PixelPoint out2, PixelPoint point1, PixelPoint point11)
{
	int a = out2.x - out1.x;
	int b = point1.x - point11.x;
	int c = out2.y - out1.y;
	int d = point1.y - point11.y;
	int g = point1.x - out1.x;
	int h = point1.y - out1.y;
	int f = a * d - b * c;
	double t = double((d * g - b * h)) / double(f);
	PixelPoint m;
	m.x = out1.x + t * (out2.x - out1.x);
	m.y = out1.y + t * (out2.y - out1.y);
	return m;
}
//中点裁剪算法
void Raster::MidPointClipAlgorithm(PixelPoint pt1, PixelPoint pt2, Layer* container) {
	//确定裁剪端口矩形
	PixelPoint rect[2];
	rect[0].x = pt1.x < pt2.x ? pt1.x : pt2.x;
	rect[0].y = pt1.y < pt2.y ? pt1.y : pt2.y;
	rect[1].x = pt1.x > pt2.x ? pt1.x : pt2.x;
	rect[1].y = pt1.y > pt2.y ? pt1.y : pt2.y;
	vector<Geometry2D*> TailorGeo;
	int i = 0;
	//遍历图层
	while (i < container->getGeometryCount()) {
		//线段的两个端点Lpt1,Lpt2
		PixelPoint Lpt1, Lpt2;
		Geometry* pGeometry = (*container)[i];
		if (pGeometry->getGeomType() != gtPolyline)
		{
			i++;
			continue;
		}
		PolylineGeometry* pPolylineGeometry = (PolylineGeometry*)pGeometry;
		const vector<Point2D>& pts = pPolylineGeometry->getPts();
		Lpt1.x = pts[0].x, Lpt1.y = pts[0].y;
		Lpt2.x = pts[1].x, Lpt2.y = pts[1].y;
		int codeLpt1 = LptOUTERrect(Lpt1, rect);
		int codeLpt2 = LptOUTERrect(Lpt2, rect);
		PixelPoint out1, out2, inn1, inn2, mid;
		if (codeLpt1 == 0 && codeLpt2 == 0) {
			//线段两个端点都在矩形内
			inn1 = Lpt1;
			inn2 = Lpt2;
		}
		if ((codeLpt1 == 1 && codeLpt2 == 0) || (codeLpt1 == 0 && codeLpt2 == 1)) {
			//线段只有一个端点在矩形内
			if (codeLpt1) {
				//Lpt1在外部
				inn1 = Lpt2;
				out1 = Lpt1;

			}
			else {
				//Lpt1在内部
				inn1 = Lpt1;
				out1 = Lpt2;
			}
			inn2 = BSearch_findIntersection(out1, inn1, rect);
		}
		if (codeLpt1 == 1 && codeLpt2 == 1) {
			//线段端点都在矩形外
			out1 = Lpt1;
			out2 = Lpt2;
			inn1 = BSearch_findIntersection(out1, out2, rect);
			inn2 = BSearch_findIntersection(out2, out1, rect);
			//利用矩形对角线与线段的交点diagonal_pt代替线段的中点mid
			//定义矩形的四个顶点abcd，则对角线为ad和bc
			PixelPoint a, b, c, d;
			a.x = rect[0].x, a.y = rect[1].y;
			b = rect[0];
			c = rect[1];
			d.x = rect[1].x, d.y = rect[0].y;
			int codeDiagonal1, codeDiagonal2;
			codeDiagonal1 = judgeParallel(out1, out2, a, d);
			codeDiagonal2 = judgeParallel(out1, out2, b, c);
			if (codeDiagonal1 == 0 || codeDiagonal2 == 0) {
				//相交，求交点
				PixelPoint diagonal_pt;
				if (!codeDiagonal1)
					diagonal_pt = getPoint(out1, out2, a, d);
				else
					diagonal_pt = getPoint(out1, out2, b, c);
				inn1 = inn2 = diagonal_pt;
				inn1 = BSearch_findIntersection(out1, inn1, rect);
				inn2 = BSearch_findIntersection(out2, inn2, rect);
			}
			else
			{
				//出现这种情况的原因是线段与线框没有交点，所以将inn都设置为空
				inn1.x = inn1.y = inn2.x = inn2.y = 999;
			}
		}
		if (inn1.x != 999 && inn2.x != 999) {
			PolylineGeometry* nPolylineGeometry = new PolylineGeometry();
			nPolylineGeometry->addPoint(inn1.x, inn1.y);
			nPolylineGeometry->addPoint(inn2.x, inn2.y);
			TailorGeo.push_back(nPolylineGeometry);
			//container->DeleteGeo(i);
		}
		container->DeleteGeo(i);
	}
	if (TailorGeo.size() == 0) return;
	//container->DeleteGeometry();
	for (int i = 0; i < TailorGeo.size(); i++)
	{
		container->addGeometry(TailorGeo[i]);
	}
}





//多边形的裁剪
//构建一个新的点的类
struct point
{
public:
	double x, y;
	point()
	{
		this->x = this->y = 0;
	}
	point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	void setX(double x)
	{
		this->x = x;
	}
	void setY(double y)
	{
		this->y = y;
	}
	double getX()
	{
		return this->x;
	}
	double getY()
	{
		return this->y;
	}
};
//再定义一个向量的类
struct VectorLine :point
{
	VectorLine(point start, point end)
	{
		this->start = start;
		this->end = end;
	}
	point start;   //开始顶点
	point end; //终结点
};
double Multi(point pt, point v_start, point v_end) {
	int x1 = pt.x - v_start.x;
	int y1 = pt.y - v_start.y;
	int x2 = v_end.x - v_start.x;
	int y2 = v_end.y - v_start.y;
	int km = x1 * y2 - x2 * y1;
	//P(x1,y1),Q(x2,y2)
	//P×Q>0,P在Q的顺时针方向
	//P×Q<0,P在Q的逆时针方向
	return km;
}
//点是否在边界线内侧
int isInside(point& pt, VectorLine& v) {
	int k = Multi(pt, v.start, v.end) >= 0 ? 0 : 1;
	//由于裁剪矩形向量是逆时针的，左侧即在矩形内部，即Multi>0在内侧，返回1
	return k;
}
//求边界与多边形边的交点
point IntersectionPt(point startPt1, point endPt1, point startPt2, point endPt2) {
	double pX = (Multi(startPt1, endPt2, startPt2) * endPt1.x - Multi(endPt1, endPt2, startPt2) * startPt1.x) /
		(Multi(startPt1, endPt2, startPt2) - Multi(endPt1, endPt2, startPt2));
	double pY = (Multi(startPt1, endPt2, startPt2) * endPt1.y - Multi(endPt1, endPt2, startPt2) * startPt1.y) /
		(Multi(startPt1, endPt2, startPt2) - Multi(endPt1, endPt2, startPt2));
	return point(pX, pY);
}
//凸多边形裁剪算法
//rectPt1为矩形裁剪框的左上角顶点
//rectPt2为矩形裁剪框的右下角顶点
void Raster::Sutherland_Hodgeman(PixelPoint rectPt1, PixelPoint rectPt2, Layer* container) {
	//确定裁剪范围
	int x_min = rectPt1.x;
	int x_max = rectPt2.x;
	int y_min = rectPt2.y;
	int y_max = rectPt1.y;
	vector<point>points;
	vector<Geometry2D*> TailorGeo;
	int i = 0;
	//遍历图层
	while (i < container->getGeometryCount()) {
		Geometry2D* pGeometry = (*container)[i];
		if (pGeometry->getGeomType() != gtPolygon)
		{
			i++;
			continue;
		}
		PolygonGeometry* pPolygonGeometry = (PolygonGeometry*)pGeometry;
		const vector<Point2D>& pts = pPolygonGeometry->getPts();
		if (pts.size() > 0)
		{
			for (int j = 0; j < pts.size(); j++)
				points.push_back(point(pts[j].x, pts[j].y)); //将点存在一个LIST中
		}
		points.push_back(point(pts[0].x, pts[0].y));
		//裁剪矩形边界改为向量，逆时针
		vector<VectorLine> vectors;
		vectors.push_back(VectorLine(point(x_max, y_min), point(x_max, y_max)));
		vectors.push_back(VectorLine(point(x_max, y_max), point(x_min, y_max)));
		vectors.push_back(VectorLine(point(x_min, y_max), point(x_min, y_min)));
		vectors.push_back(VectorLine(point(x_min, y_min), point(x_max, y_min)));
		vector<point> result, cur;
		int vectorsSize = vectors.size();
		int pointSize = points.size();
		if (points.size() > 0) {
			//点S为多边形存储的最后一个顶点，即为判断内外的起始点
			//point S = points[points.size()-1];
			point S = points[0];

			for (int j = 0; j < points.size(); ++j)
				result.push_back(points[j]);
			//flag：S在边界内侧，在就返回1
			int flag;
			//从最右边边开始，右上左下
			for (int j = 0; j < vectors.size(); ++j)
			{
				//先判断起始点S是否在边界线内
				if (isInside(S, vectors[j]))
					flag = 1;
				else
					flag = 0;
				//当前矩形边时的所有多边形的点是否在边的内侧
				int resultSize = result.size();
				for (int k = 0; k < resultSize; ++k)
				{
					//当前点在内侧
					if (isInside(result[k], vectors[j]))
					{
						//flag表示前一个点，flag==1表示在内侧
						if (flag == 0)
						{
							//前一个点在外侧，当前点在内侧，求交点并存入cur中
							flag = 1;
							if (k == 0)
								cur.push_back(IntersectionPt(S, result[k], vectors[j].start, vectors[j].end));
							else
								cur.push_back(IntersectionPt(result[k - 1], result[k], vectors[j].start, vectors[j].end));
							cur.push_back(result[k]);
						}
						else {
							//两个点都在内侧，直接保存当前点到cur中
							cur.push_back(result[k]);
						}
					}
					else {//当前点在外侧
						if (flag) {
							//前一个点在内侧，求交点并存入cur中
							flag = 0;
							if (k == 0)
								cur.push_back(IntersectionPt(S, result[k], vectors[j].start, vectors[j].end));
							else
								cur.push_back(IntersectionPt(result[k - 1], result[k], vectors[j].start, vectors[j].end));
						}
						//两个点都在外侧
						//将当前顶点赋给S，新的输入点
						//S = result[k];
					}
				}
				//if (j != 0) {
				//	//向cur中添加上一步未添加的S点
				//	cur.push_back(S);
				//}
				//cur.push_back(S);
				if (!cur.size())
				{
					result.clear();
					break;
				}
				if (cur[0].x != cur[cur.size() - 1].x || cur[0].y != cur[cur.size() - 1].y)
				{
					cur.push_back(cur[0]);
				}

				if (j == vectors.size() - 1) {
					//结束裁剪，将首尾点相连
					result.clear();
					for (int k = 0; k < cur.size()-1; ++k) {
						result.push_back(cur[k]);
					}
					//result.push_back(cur[0]);
					cur.clear();
				}
				else {
					S = cur[0];
					int reaultLen = cur.size();
					result.clear();
					for (int k = 0; k < reaultLen; ++k)
					{
						result.push_back(cur[k]);
					}
					cur.clear();
				}
			}

			int K = result.size();
			if (K < 3) continue;
			PolygonGeometry* npPolygonGeometry = new PolygonGeometry();
			for (int j = 0; j < K; ++j)
			{
				npPolygonGeometry->addPoint(result[j].x, result[j].y);
			}
			TailorGeo.push_back(npPolygonGeometry);
			result.clear();
			vectors.clear();
			points.clear();
		}
		container->DeleteGeo(i);
	}
	if (TailorGeo.size() == 0) return;
	//container->DeleteGeometry();
	for (int i = 0; i < TailorGeo.size(); i++)
	{
		container->addGeometry(TailorGeo[i]);
	}
}


/// <summary>
/// 两条边求交点
/// </summary>
/// <param name="subS">被裁减边起点</param>
/// <param name="subE">被裁剪边终点</param>
/// <param name="clipS">裁剪边起点</param>
/// <param name="clipE">裁剪边终点</param>
/// <returns></returns>
IntersectionPoint findIntersection(Point2D& subS,Point2D& subE, Point2D& clipS,Point2D& clipE)
{
	double a1 = subE.y - subS.y;
	double b1 = subS.x - subE.x;
	double c1 = a1 * subS.x + b1 * subS.y;

	double a2 = clipE.y - clipS.y;
	double b2 = clipS.x - clipE.x;
	double c2 = a2 * clipS.x + b2 * clipS.y;

	// 叉乘结果判断出入点，正为入点，负为出点
	double isEnter = a1 * b2 - a2 * b1;
	if (isEnter == 0) { return IntersectionPoint(); }
	else
	{
		double x = (b2 * c1 - b1 * c2) / isEnter;
		double y = (a1 * c2 - a2 * c1) / isEnter;
		if (x < min(subS.x, subE.x) || x > max(subS.x, subE.x)) return IntersectionPoint();
		if (y < min(subS.y, subE.y) || y > max(subS.y, subE.y)) return IntersectionPoint();
		if (x < min(clipS.x, clipE.x) || x > max(clipS.x, clipE.x)) return IntersectionPoint();
		if (y < min(clipS.y, clipE.y) || y > max(clipS.y, clipE.y)) return IntersectionPoint();
		return IntersectionPoint(x, y, isEnter > 0);
	}
}

/// <summary>
/// 出入点按顺序插入两个多边形中
/// </summary>
/// <param name="pts">要插入的多边形点集</param>
/// <param name="startPt">交点边的起点</param>
/// <param name="endPt">交点边的终点</param>
/// <param name="ip">被插入的点</param>
void addPointToPolygon(vector<Point2D>& pts,Point2D& startPt,Point2D& endPt, IntersectionPoint& ip)
{
	auto start = find(pts.begin(), pts.end(), startPt);
	auto end = find(pts.begin(), pts.end(), endPt);
	if ((Point2D)ip == *start || (Point2D)ip == *end) return;

	auto it = start;
	double dist_from_start = distance(ip, *it);
	while (it != end && it != pts.end())
	{
		// 找到插入点位置，跳出循环
		if (distance(*it, *start) >= dist_from_start) break;
		it++;
	}
	pts.insert(it, ip);
}

/// <summary>
/// 多边形求交点，输出交点、入点，并添加到多边形顶点集中
/// </summary>
/// <param name="subj">被裁减多边形顶点集合</param>
/// <param name="clip">裁剪多边形顶点集合</param>
/// <param name="enter_pts">入点集合</param>
/// <param name="exit_pts">出点集合</param>
void getIntersectionPoints(vector<Point2D>& subj, vector<Point2D>& clip, vector<Point2D>& enter_pts, vector<Point2D>& exit_pts)
{
	vector<Point2D> subj_pts(subj);
	vector<Point2D> clip_pts(clip);

	int subSize = subj_pts.size();
	int clipSize = clip_pts.size();

	// 外循环，遍历被裁减多边形的边
	for (int i = 0; i < subSize; i++)
	{
		Point2D subStartPt = subj_pts[i];
		Point2D subEndPt = subj_pts[(i + 1)%subSize];

		// 内循环，遍历裁剪多边形的边
		for (int j = 0; j < clipSize; j++)
		{
			Point2D clipStartPt = clip_pts[j];
			Point2D clipEndPt = clip_pts[(j + 1) % clipSize];

			// 求交点
			IntersectionPoint ip = findIntersection(subStartPt,subEndPt,clipStartPt,clipEndPt);


			if (ip.isIntersectionPoint())
			{
				if (ip.isEntering())
				{
					enter_pts.push_back(ip);
				}
				else
				{
					exit_pts.push_back(ip);
				}
				addPointToPolygon(subj, subStartPt, subEndPt, ip);
				addPointToPolygon(clip, clipStartPt, clipEndPt, ip);
			}
		}
	}
}
/// <summary>
/// 从入点开始移动搜索相交多边形顶点
/// </summary>
/// <param name="enter_pts">入点集合</param>
/// <param name="exit_pts">出点集合</param>
/// <param name="search_pts">搜索的点集</param>
/// <param name="polygon">相交多边形</param>
/// <param name="start">搜索起点</param>
/// <returns></returns>
Point2D moveSearch(vector<Point2D>& enter_pts, vector<Point2D>& exit_pts,vector<Point2D>& search_pts, PolygonGeometry* polygon, Point2D& start)
{
	// 找到搜索起点在被搜索点集中的位置
	auto iter = find(search_pts.begin(), search_pts.end(), start);

	while (true)
	{
		polygon->addPoint((*iter).x,(*iter).y);
		iter++;
		if (iter == search_pts.end()) iter = search_pts.begin();

		// 如果下一个点在入点集合或者出点集合中则跳出循环
		auto it = find(enter_pts.begin(), enter_pts.end(), *iter);
		if (it != enter_pts.end())
		{
			// 删除入点集中该入点
			enter_pts.erase(it);
			break;
		}
		auto it_exit = find(exit_pts.begin(), exit_pts.end(), *iter);
		if (it_exit != exit_pts.end())
		{
			exit_pts.erase(it_exit);
			break;
		}
	}
	// 返回下一个点
	return *iter;

}

/// <summary>
/// 处理交点为顶点的情况
/// </summary>
/// <param name="enterPts">入点</param>
/// <param name="exitPts">出点</param>
void confirmVertexIntersectionPoints(vector<Point2D>& enterPts, vector<Point2D>& exitPts)
{
	// 出点去重
	set<Point2D> enter_pts(enterPts.begin(), enterPts.end());
	enterPts.assign(enter_pts.begin(), enter_pts.end());

	// 入点去重
	set<Point2D> exit_pts(exitPts.begin(), exitPts.end());
	exitPts.assign(exit_pts.begin(), exit_pts.end());

	// 删除同时在出入点集合中的点

	for (auto itEnter = enterPts.begin();itEnter!=enterPts.end();)
	{

		auto iter = find(exitPts.begin(), exitPts.end(), *itEnter);
		if (iter != exitPts.end())
		{
			exitPts.erase(iter);
			itEnter = enterPts.erase(itEnter);
		}
		else itEnter++;
	}
}

/// <summary>
/// 求射线交点坐标
/// </summary>
/// <param name="y"></param>
/// <param name="StartPt"></param>
/// <param name="EndPt"></param>
/// <returns></returns>
int getXcoordinates(int y, Point2D& StartPt, Point2D& EndPt)
{
	double a = EndPt.y - StartPt.y;
	double b = StartPt.x - EndPt.x;
	double c = a * StartPt.x + b * StartPt.y;
	return (c - b * y) / a;
}
/// <summary>
/// 检测点是否在坐标内部
/// </summary>
/// <param name="pt"></param>
/// <param name="polygonVertex"></param>
/// <returns></returns>
bool isInClipPolygonPts(Point2D& pt, vector<Point2D>& polygonVertex)
{
	int size = polygonVertex.size();
	int IntersectionSum = 0;
	vector<int> coordsX;
	// 遍历多边形的边
	for (int i = 0; i < size; i++)
	{
		Point2D subStartPt = polygonVertex[i];
		Point2D subEndPt = polygonVertex[(i + 1) % size];
		if(pt.y < min(subStartPt.y,subEndPt.y) || pt.y > max(subStartPt.y,subEndPt.y)) continue;
		int x = getXcoordinates(pt.y, subStartPt, subEndPt);
		if (x < pt.x) coordsX.push_back(x);
	}
	if (coordsX.size() % 2 == 0) return false;
	return true;
}

PolygonGeometry* getAllClipPolygon(vector<Point2D>& subPts, vector<Point2D>& clipPts)
{
	bool isInClip = false;
	PolygonGeometry* polygon = new PolygonGeometry();
	for (auto pt : subPts)
	{
		// 被裁减多边形是否在裁剪多变形内部
		if (isInClipPolygonPts(pt, clipPts))
		{
			polygon->initPts(subPts);
			isInClip = true;
			break;
		}
	}
	if (!isInClip)
	{
		for (auto pt : clipPts)
		{
			// 裁剪多边形是否在被裁剪多边形内部
			if (isInClipPolygonPts(pt, subPts))
			{
				polygon->initPts(clipPts);
				break;
			}
		}
	}
	return polygon;
}

/// <summary>
/// Weiler-Atherton算法裁剪多边形
/// </summary>
/// <param name="subject">被裁剪多边形</param>
/// <param name="clipping">裁剪多边形</param>
/// <returns></returns>
vector<PolygonGeometry*> Raster::WeilerAtherton(PolygonGeometry* subject, PolygonGeometry* clipping)
{
	vector<PolygonGeometry*> result;
	if (subject->getPts().size() < 3 || clipping->getPts().size() < 3) return result;
	if (!subject->isClockWiseDirection()) subject->reverseDirection();
	if (!clipping->isClockWiseDirection()) clipping->reverseDirection();

	vector<Point2D> subj_points(subject->getPts());
	vector<Point2D> clip_points(clipping->getPts());
	vector<Point2D> enter_points;
	vector<Point2D> exit_points;
	
	// 求交点
	getIntersectionPoints(subj_points, clip_points, enter_points, exit_points);
	// 处理交点在顶点处的情况
	confirmVertexIntersectionPoints(enter_points,exit_points);
	// 处理无交点情况
	if (enter_points.size() == 0 && exit_points.size() == 0)
	{
		if (getAllClipPolygon(subj_points, clip_points)->getPts().size() != 0)
		{
			result.push_back(getAllClipPolygon(subj_points, clip_points));
		}
		return result;
	}
	

	auto iter_enter_point = enter_points.begin();

	// 从第一个入点开始搜索，直到所有入点清除完退出
	while (iter_enter_point != enter_points.end())
	{
		//if(find(subj_points.begin(),subj_points.end(),*iter_enter_point) == subj_points.end()) continue;
		PolygonGeometry* polygon = new PolygonGeometry();
		Point2D start = *iter_enter_point, next = start;

		do
		{
			// 入点开始搜索，遇出点返回
			next = moveSearch(enter_points, exit_points, subj_points, polygon, next);
			// 出点开始搜素，遇入点返回
			next = moveSearch(enter_points, exit_points, clip_points, polygon, next);
		} while (next != start); // 搜素到起始入点，结束一个相交区域的搜索;
		result.push_back(polygon);
		iter_enter_point = enter_points.begin(); // 重新指向入点集起始位置
	}
	
	return result;
}

void drawTriangleZ(vector3D* pts, unsigned color) 
{
	vector3D t0 = pts[0];
	vector3D t1 = pts[1];
	vector3D t2 = pts[2];

	if (t0.y == t1.y && t0.y == t2.y) return;

	//t0,t1,t2 由高到低为三角形的三个顶点
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);

	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		//second_half代表三角形一分为二的上和下，1为上半部分，0为下半部分
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		//segment_height为过t1的水平线与t2和t0的距离，即为两个小三角形的高
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		//α为t0和t2与水平线的夹角，β为t2和t1与水平线的夹角
		double alpha = (double)i / total_height;
		double beta = (double)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
		//A为t0和t2的连线，B为t0和t1的连线
		vector3D A = t0 + (t2 - t0) * alpha;
		vector3D B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x,k=A.z; j <= B.x; j++,k++) {
			//g_State.drawPixelCB(j, t0.y + i, BLACK);
			setPixel(j, t0.y+i, k,color);
		}
	}
}
double ToGetDeepthZ(vector3D pt0, vector3D pt1, vector3D pt2, int x, int y)
{
	// 多边形平面方程
	// Ax+By+Cz+D=0
	// z(x,y)=(-Ax-By-D)/C

	// 平面的法向量(A,B,C)
	//| i      j    k   |
	//|x2-x1 y2-y1 z2-z1| //mx my mz
	//|x3-x1 y3-y1 z3-z1| //nx ny nz
	double mx = pt1.x - pt0.x;
	double my = pt1.y - pt0.y;
	double mz = pt1.z - pt0.z;

	double nx = pt2.x - pt0.x;
	double ny = pt2.y - pt0.y;
	double nz = pt2.z - pt0.z;

	double A = my * nz - ny * mz;
	double B = nx * mz - mx * nz;
	double C = mx * ny - nx * my;
	double D = -(A * pt0.x + B * pt0.y + C * pt0.z);

	if (C == 0)//此时多边形在xoy平面上的投影为线，不考虑
		return 9999;
	double  z = (-A * x - B * y - D) / C;
	return z;
}
void Raster::Draw3DTriangle(vector3D& p1, vector3D& p2, vector3D& p3, Color color)
{
	vector<vector3D> face;
	/*drawTriangleZ(face.data(),color);
	return;*/
	face.push_back(p1);
	face.push_back(p2);
	face.push_back(p3);
	int size = 3;
	int miny = -9999;
	int maxy = 9999;
	for (int i = 0; i < size; i++)
	{
		if (face[i].y < miny)
			miny = face[i].y;
		if (face[i].y > maxy)
			maxy = face[i].y;
	}
	for (int y = miny; y < maxy; y++) 
	{
		//一个数组用来存储本次扫描线交点信息
		vector<int> gvec;
		//判断本次扫描线是否与面上的各个线相交
		for (int i = 0; i < size; i++) {
			if (y == int(face[i].y)) {
				//扫到面上的一个顶点
				gvec.push_back(face[i].x);
			}
			//扫描线在顶点之间时
			if ((y > int(face[i].y) && y < int(face[(i + 1) % size].y)) || (y < int(face[i].y) && y > int(face[(i + 1) % size].y))) {
				int x = face[i].x + (y - face[i].y) * (face[(i + 1) % size].x - face[i].x) / (face[(i + 1) % size].y - face[i].y);
				gvec.push_back(x);
			}
		}
		// 将本次扫描所有交点的x值进行排序（三角形最多两个）
		sort(gvec.begin(), gvec.end());
		if (gvec.size() < 2)
			continue;
		for (int x = gvec[0]; x < gvec[1]; x++) 
		{
			double deepz = ToGetDeepthZ(face[0], face[1], face[2], x, y);
			setPixel(x, y, deepz, color);
		}
	}

}

struct FlatTri
{
	double top; // 三角形底
	double bottom; // 三角形顶
	texture_v left_v1; // 三角形左边
	texture_v right_v2;
	texture_v peek_v; // 三角形右边
	bool isUp;
};

// 线性插值
double Interplotaion(double x1, double x2,double t)
{
	return x1 + (x2 - x1) * t;
}
void Division_h(texture_v& v)
{
	double w = 1.0 / v.pos.h;
	v.pos.h = w;
	v.tex.x *= w;
	v.tex.y *= w;
	v.RGB.r *= w;
	v.RGB.g *= w;
	v.RGB.b *= w;
	v.Light *= w;
}
texture_v getLeftToRight(texture_v& p1, texture_v& p2,double t)
{
	texture_v newTexture;
	newTexture.pos.x = Interplotaion(p1.pos.x, p2.pos.x, t);
	if (abs(newTexture.pos.x - p1.pos.x) > abs(p2.pos.x - p1.pos.x))
	{
		newTexture.pos.x = p2.pos.x;
	}
	newTexture.pos.y = Interplotaion(p1.pos.y, p2.pos.y, t);
	newTexture.pos.z = Interplotaion(p1.pos.z, p2.pos.z, t);
	newTexture.pos.h = Interplotaion(p1.pos.h, p2.pos.h, t);

	newTexture.tex.x = Interplotaion(p1.tex.x, p2.tex.x, t);
	newTexture.tex.y = Interplotaion(p1.tex.y, p2.tex.y, t);

	newTexture.RGB.r = Interplotaion(p1.RGB.r, p2.RGB.r,t);
	newTexture.RGB.g = Interplotaion(p1.RGB.g, p2.RGB.g, t);
	newTexture.RGB.b = Interplotaion(p1.RGB.b, p2.RGB.b, t);
	newTexture.Light = Interplotaion(p1.Light, p2.Light, t);
	return newTexture;
}
texture_v CalculateAdd(texture_v& pl, texture_v& pr, double w)
{
	texture_v newTexture;
	w = 1.0 / w;
	newTexture.pos.x = (pr.pos.x - pl.pos.x) * w;
	newTexture.pos.y = (pr.pos.y - pl.pos.y) * w;
	newTexture.pos.z = (pr.pos.z - pl.pos.z) * w;
	newTexture.pos.h = (pr.pos.h - pl.pos.h) * w;

	newTexture.tex.x = (pr.tex.x - pl.tex.x) * w;
	newTexture.tex.y = (pr.tex.y - pl.tex.y) * w;

	newTexture.RGB.r = (pr.RGB.r - pl.RGB.r) * w;
	newTexture.RGB.g = (pr.RGB.g - pl.RGB.g) * w;
	newTexture.RGB.b = (pr.RGB.b - pl.RGB.b) * w;
	newTexture.Light = (pr.Light- pl.Light) * w;

	return newTexture;
}
unsigned Raster::read_Color(double u, double v)
{
	int x = (int)(u * (texture_w) + 0.5);
	if (x < 0) x = 0;
	else if (x > 255) x = x % 255;
	int y = (int)(v * (texture_h) + 0.5);
	if (y < 0) y = 0;
	else if (y > 255) y = y % 255;
	return pImageHeader[y][x];
	//return texture[y][x];
}
void Draw_scan_line(FlatTri* trap, int y)
{
	double t_left;
	double t_right;
	texture_v left_v; // 扫描线左端点，插值获得
	texture_v right_v; // 扫描线右端点，插值获得
	if (!trap->isUp)
	{
		// 下三角形
		t_left = ((y+0.5) * 1.0 - trap->peek_v.pos.y) / (trap->left_v1.pos.y - trap->peek_v.pos.y);
		t_right = ((y+0.5) * 1.0 - trap->peek_v.pos.y) / (trap->right_v2.pos.y - trap->peek_v.pos.y);
		left_v = getLeftToRight(trap->peek_v, trap->left_v1, t_left);
		right_v = getLeftToRight(trap->peek_v, trap->right_v2, t_right);
	}
	else
	{
		// 上三角形
		t_left = ((y + 0.5) * 1.0 - trap->left_v1.pos.y) / (trap->peek_v.pos.y - trap->left_v1.pos.y);
		t_right = ((y + 0.5) * 1.0 - trap->right_v2.pos.y) / (trap->peek_v.pos.y - trap->right_v2.pos.y);

		// 计算左右两边端点
		left_v = getLeftToRight(trap->left_v1, trap->peek_v, t_left);
		right_v = getLeftToRight(trap->right_v2, trap->peek_v, t_right);
	}
	
	// 
	double width = right_v.pos.x - left_v.pos.x;
	if (width <= 0) width = 0;
	// 计算属性增量
	texture_v addtexture = CalculateAdd(left_v, right_v, width);
	texture_v vs = left_v; // 扫描移动点

	int x = (int)(left_v.pos.x+0.5); // 左边起始x坐标
	int xr = (int)(right_v.pos.x + 0.5); // 右边终止x坐标
	int w = xr - x;
	for (; w>0; x++,w--)
	{
		double w = 1.0 / vs.pos.h; // 获取Z值
		if (g_State.g_3DState == Texture)
		{
			double u = vs.tex.x * w;  // 乘以Z值，获取属性值
			double v = vs.tex.y * w;
			unsigned cc = Raster::read_Color(u, v); // 根据uv坐标获取颜色
			setPixel(x, y, w, cc); // 着色
		}
		if (g_State.g_3DState == LightGourand)
		{
			double r = vs.RGB.r * w;
			double g = vs.RGB.g * w;
			double b = vs.RGB.b * w;
			double L = vs.Light * w;
			if (r < 0) r = 0;
			else if (r > 255) r = 255;
			if (g < 0) g = 0;
			else if (g > 255) g = 255;
			if (b < 0) r = 0;
			else if (b > 255) g = 255;
			setPixel(x, y, w, _RGB(r,g,b));
		}

		vs += addtexture; // 加增量
	}

}
void RenderTriangle(FlatTri* trap)
{
	int top = (int)(trap->top+0.5);
	int bottom = (int)(trap->bottom+0.5);

	// 自底向上，依次进行扫描
	for (int y = bottom; y < top; y++)
	{
		Draw_scan_line(trap, y);
	}
}
void Swap_texture(texture_v& v1, texture_v& v2)
{
	std::swap(v1.pos, v2.pos);
	std::swap(v1.tex, v2.tex);
	std::swap(v1.RGB, v2.RGB);
	std::swap(v1.Light, v2.Light);
}


int BreakUp_Triangle(FlatTri* trap, texture_v& p1, texture_v& p2, texture_v& p3)
{
	// 顶点p1,p2,p3按照y值由小到大排序
	if (p1.pos.y > p2.pos.y) 
		Swap_texture(p1, p2);
	if (p1.pos.y > p3.pos.y) Swap_texture(p1, p3);
	if (p2.pos.y > p3.pos.y) Swap_texture(p2, p3);
	if (p1.pos.y == p2.pos.y && p2.pos.y == p3.pos.y) return 0;
	if (p1.pos.x == p2.pos.x && p2.pos.x == p3.pos.x) return 0;

	if (p1.pos.y == p2.pos.y)
	{
		// left_v1为平底左端点，right_v2为平底右端点
		if (p1.pos.x > p2.pos.x) Swap_texture(p1, p2);
		trap[0].top = p3.pos.y;
		trap[0].bottom = p1.pos.y;
		trap[0].left_v1 = p1;
		trap[0].right_v2 = p2;
		trap[0].peek_v = p3;
		trap[0].isUp = true;
		return (trap[0].top > trap[0].bottom) ? 1 : 0;
	}

	if (p2.pos.y == p3.pos.y)
	{
		if (p2.pos.x > p3.pos.x) Swap_texture(p2, p3);
		trap[0].top = p3.pos.y;
		trap[0].bottom = p1.pos.y;
		trap[0].left_v1 = p2;
		trap[0].right_v2 = p3;
		trap[0].peek_v = p1;
		trap[0].isUp = false;
		return (trap[0].top > trap[0].bottom) ? 1 : 0;
	}

	trap[0].top = p3.pos.y;
	trap[0].bottom = p2.pos.y;
	trap[0].isUp = true;

	trap[1].top = p2.pos.y;
	trap[1].bottom = p1.pos.y;
	trap[1].isUp = false;

	double k = (p3.pos.y - p1.pos.y) / (p2.pos.y - p1.pos.y);
	double x = p1.pos.x + (p2.pos.x - p1.pos.x) * k;

	if (x <= p3.pos.x)
	{
		// 左三角形
		trap[0].left_v1 = p2;
		trap[0].right_v2 = p1;
		trap[0].peek_v = p3;

		trap[1].left_v1 = p2;
		trap[1].right_v2 = p3;
		trap[1].peek_v = p1;
	}
	else
	{
		// 右三角形
		trap[0].left_v1 = p1;
		trap[0].right_v2 = p2;
		trap[0].peek_v = p3;
		
		trap[1].left_v1 = p3;
		trap[1].right_v2 = p2;
		trap[1].peek_v = p1;
		
	}
	return 2;

}

/// <summary>
/// 进行插值
/// </summary>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="p3"></param>
void Raster::Draw3DTriangle(texture_v& p1, texture_v& p2, texture_v& p3)
{
	// 平底三角形，isUp判断方向向上还是向下
	FlatTri traps[2];

	// 除以Z值，以便获得正确的线性插值
	// 因为透视投影后，投影到平面上的属性值是非线性的，但Z值的倒数是线性的，因此端点的属性值要除以Z，
	Division_h(p1);
	Division_h(p2);
	Division_h(p3);

	// 拆分为两个平底三角形,返回拆分结果个数
	int n = BreakUp_Triangle(traps, p1, p2, p3);
	if (n == 0) return;
	if (n == 1)
	{
		RenderTriangle(&traps[0]);
		return;
	}
	RenderTriangle(&traps[0]);
	RenderTriangle(&traps[1]);
}

void Raster::texture_init()
{
	texture_w = 256;
	texture_h = 256;
	pImage = new unsigned[texture_w * texture_h * 4];
	pImageHeader = new unsigned* [256];
	switch (texture_mode)
	{
	case 0:
		FractalImageGenerator::generateJuliaSetImage(pImage, texture_h, texture_w);
		break;
	case 1:
		FractalImageGenerator::generateMandelbrotImage(pImage, texture_h, texture_w);
		break;
	default:
		FractalImageGenerator::generateJuliaSetImage(pImage, texture_h, texture_w);
		break;
	}
	//FractalImageGenerator::generateJuliaSetImage(pImage, texture_h, texture_w);
	char* ptr = (char*)pImage;
	for (int i = 0; i < 256;ptr += (256*4), i++)
	{
		pImageHeader[i] = (unsigned*)ptr;
	}
	pHeader = pImage;
	
}

//}