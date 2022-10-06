#include <windows.h>
#include <windowsx.h>
#include "MyGraphic.h"
#include "Graphic.h"
#include <vector>
#include "DrawState.h"
#include <algorithm>
#include <stack>
//using namespace State;
using namespace std;
extern Color g_Color; // �߶���ɫ
extern PixelPoint g_Points[100]; // �����Ҫ���Ƶĵ������
extern int g_PointCout;  //��ĸ���

int sign(int val)
{
	if (val > 0)
		return 1;
	else if (val < 0)
		return -1;
	else
		return 0;
}
/// <summary>
/// Bresenham�е��㷨
/// </summary>
/// <param name="startp">��ʼ�����������</param>
/// <param name="endp">�����յ�����</param>
void MidPointLine(double x0, double y0, double x1, double y1, unsigned color)
{
	int dex = abs(x0 - x1);
	int dey = abs(y0 - y1);
	bool tag = false;
	// ��ֱ��б�ʴ���1ʱ
	if (dex < dey)
	{
		std::swap(dex, dey);
		tag = true;
	}
	// x,y����
	int dx = sign(x1 - x0); int dy = sign(y1 - y0);
	int d = dex - dey - dey; // ��ʼ����
	int x = x0; int y = y0; // ���
	for (int i = 0; i <= dex; ++i)
	{
		g_State.drawPixelCB(x, y, color);
		if (d < 0) // d<0ʱ��x,y�ֱ���ϼ�dx,dy
		{
			x += dx; y += dy;
			d += (dex + dex - dey - dey); // ��������2dex-2dey
		}
		else
		{
			// d>0��б�ʴ���1ʱ��y����
			if (tag == true)
				y += dy;
			else // d>0��б�ʲ�����1ʱ��x����
				x += dx;
			d -= (dey + dey); // ��������-2dey
		}
	}
}
/// <summary>
/// DDAֱ���㷨
/// </summary>
/// <param name="startp">��ʼ�����������</param>
/// <param name="endp">�����յ�����</param>
void DDALine(double x0, double y0, double x1, double y1, unsigned color)
{
	int x = 0, dex, dey, dexx, deyy;
	float dx, dy; // ����
	dexx = x1 - x0;
	deyy = y1 - y0;
	dex = abs(dexx);
	dey = abs(deyy);
	dx = (float)dexx / max(dex, dey);
	dy = (float)deyy / max(dex, dey);
	if (dex < dey)
	{
		std::swap(dex, dey);
	}
	while (x < dex)
	{
		g_State.drawPixelCB(round(x0), round(y0), color);
		x0 += dx;
		y0 += dy;
		x++;
	}
}

/* ��Բ��Ӧ�˸���λ��*/
void CirclePlot(int xCenter, int yCenter, int x, int y, unsigned color)
{
	g_State.drawPixelCB(xCenter + x, yCenter + y, color);
	g_State.drawPixelCB(xCenter - x, yCenter + y, color);
	g_State.drawPixelCB(xCenter + x, yCenter - y, color);
	g_State.drawPixelCB(xCenter - x, yCenter - y, color);
	g_State.drawPixelCB(xCenter + y, yCenter + x, color);
	g_State.drawPixelCB(xCenter - y, yCenter + x, color);
	g_State.drawPixelCB(xCenter + y, yCenter - x, color);
	g_State.drawPixelCB(xCenter - y, yCenter - x, color);
}

/* ����Բ��Ӧ�ĸ���λ��*/
void EllipsePlot(int xCenter, int yCenter, int x, int y, unsigned color)
{
	g_State.drawPixelCB(xCenter + x, yCenter + y, color);
	g_State.drawPixelCB(xCenter - x, yCenter + y, color);
	g_State.drawPixelCB(xCenter + x, yCenter - y, color);
	g_State.drawPixelCB(xCenter - x, yCenter - y, color);
}

/*���������դ��*/
void GetYMinMax(PixelPoint* data, int size, int& ymin, int& ymax)
{
	ymin = data[0].y;
	ymax = data[0].y;
	for (int i = 1; i < size; i++)
	{
		if (data[i].y > ymax)
			ymax = data[i].y;
		if (data[i].y < ymin)
			ymin = data[i].y;
	}
}
void InitET(vector<vector<State::tagEDGE>>& etEDGE, PixelPoint* data, int size, int ymin, int ymax)
{
	State::tagEDGE edgeNode;
	// �������е�
	for (int i = 0; i < size; i++)
	{
		PixelPoint ps = data[i]; // ��ǰ�����
		PixelPoint pe = data[(i + 1) % size]; // ��ǰ���յ�
		PixelPoint pss = data[(i - 1 + size) % size]; // ������ڱ����
		PixelPoint pee = data[(i + 2) % size]; // �յ����ڱ��յ�
		// ˮƽ�߲�����
		if (ps.y != pe.y)
		{
			edgeNode.dx = double(pe.x - ps.x) / double(pe.y - ps.y);
			if (pe.y > ps.y) // ȡ��Сֵy�ĵ�x
			{
				edgeNode.x = ps.x;
				edgeNode.ymax = pee.y >= pe.y ? pe.y - 1 : pe.y;
				etEDGE[ps.y - ymin].push_back(edgeNode);
			}
			else
			{
				edgeNode.x = pe.x;
				edgeNode.ymax = pss.y >= ps.y ? ps.y - 1 : ps.y;
				etEDGE[pe.y - ymin].push_back(edgeNode);
			}
		}
	}
}

// ��������
bool LessSort(State::tagEDGE a, State::tagEDGE b)
{
	if (a.x != b.x)
	{
		return a.x < b.x;
	}

	return a.dx < b.dx;
	//return (a.x < b.x ? true : (a.dx < b.dx ? true : false));
}
void InsertAET(vector<State::tagEDGE>& e, vector<State::tagEDGE>& ae)
{
	if (e.size())
	{
		ae.insert(ae.end(), e.begin(), e.end());
		sort(ae.begin(), ae.end(), LessSort);
	}
	return;
}
void ScanFillLine(vector<State::tagEDGE>& aetEDGE, int y, unsigned color)
{
	int size = aetEDGE.size();
	int i = 1;
	while (i < size)
	{
		int x0 = aetEDGE[i - 1].x;
		int x1 = aetEDGE[i].x;
		for (int x = x0; x <= x1; x++)
		{
			g_State.drawPixelCB(x, y, color);
		}

		i += 2;
	}
}
void RemoveAET(vector<State::tagEDGE>& aetEDGE, int y)
{
	auto iter = aetEDGE.begin();
	while (iter != aetEDGE.end())
	{
		if (y == (*iter).ymax)
		{
			iter = aetEDGE.erase(iter);
		}
		else
			iter++;
	}
}

void UpdateAET(vector<State::tagEDGE>& aetEDGE)
{
	for (vector<State::tagEDGE>::iterator iter = aetEDGE.begin(); iter != aetEDGE.end(); iter++)
	{
		(*iter).x += (*iter).dx;
	}
}

void ScanLineFill(vector<State::tagEDGE>& aetEDGE, vector<vector<State::tagEDGE>>& etEDGE, int ymin, int ymax, unsigned color)
{
	// ����Сy��ʼ������ɨ�������y
	for (int y = ymin; y <= ymax; y++)
	{
		InsertAET(etEDGE[y - ymin], aetEDGE); // ����ǰ�߱����л��Աߣ��������Ա���
		ScanFillLine(aetEDGE, y, color); // �Ե�ǰ�н������
		RemoveAET(aetEDGE, y);  // ��ǰ���Ա߱����Ƿ��б��Ѿ�ɨ���꣬��Ҫ�Ƴ�
		UpdateAET(aetEDGE); // ����x����
		continue; // ������һ��ɨ��
	}
}
void HorizonEdgeFill(PixelPoint* data, int size, unsigned color)
{
	for (int i = 0; i < size; i++)
	{
		if (data[i].y == data[(i + 1) % size].y)
		{
			for (int x = data[i].x; x < data[(i + 1) % size].x; x++)
			{
				g_State.drawPixelCB(x, data[i].y, color);
			}
		}
	}
}

/*��������㷨*/
void PointFill(int x, int y, unsigned newColor)
{
	unsigned oldColor = getPixel(x, y); // ��ȡ�����ڵ�ɵ���ɫ
	if (oldColor == newColor) return;
	PixelPoint startPt = { x, y };
	PixelPoint AdjPt[4];  // �������ͨ�����ܱߵ��ĸ���
	vector<PixelPoint> AdjPtList; // ����һ��ջ���δ�������ɫ�ĵ�
	AdjPtList.push_back(startPt); // �����ӵ�ѹ��ջ

	// ��ջ��ȡ�����������ɫ���ټ���ܱ��ĸ��㣬��δ���ѹ��ջ��,ѭ��ֱ��ջΪ��
	while (!AdjPtList.empty())
	{
		PixelPoint topPt = AdjPtList.back();
		g_State.drawPixelCB(topPt.x, topPt.y, newColor);
		AdjPt[0] = { topPt.x - 1,topPt.y };
		AdjPt[1] = { topPt.x,topPt.y + 1 };
		AdjPt[2] = { topPt.x + 1,topPt.y };
		AdjPt[3] = { topPt.x, topPt.y - 1 };
		AdjPtList.pop_back();
		for (int i = 0; i < 4; i++)
		{
			if (getPixel(AdjPt[i].x, AdjPt[i].y) == oldColor)
			{
				AdjPtList.push_back(AdjPt[i]);
			}
		}
	}
}

void BoundaryFill(int x, int y, unsigned bdrColor, unsigned newColor)
{
	PixelPoint startPt = { x, y };
	PixelPoint AdjPt[4];
	vector<PixelPoint> AdjPtList;
	AdjPtList.push_back(startPt);
	while (!AdjPtList.empty())
	{
		PixelPoint topPt = AdjPtList.back();
		g_State.drawPixelCB(topPt.x, topPt.y, newColor);
		AdjPt[0] = { topPt.x - 1,topPt.y };
		AdjPt[1] = { topPt.x,topPt.y + 1 };
		AdjPt[2] = { topPt.x + 1,topPt.y };
		AdjPt[3] = { topPt.x, topPt.y - 1 };
		AdjPtList.pop_back();
		for (int i = 0; i < 4; i++)
		{
			unsigned oldColor = getPixel(AdjPt[i].x, AdjPt[i].y);
			if (oldColor != bdrColor && oldColor != newColor)
			{
				AdjPtList.push_back(AdjPt[i]);
			}
		}
	}
}

/// <summary>
/// ���������������ӵ���ջ�㷨,�����ڿ��������,�����������ұ߽���Ϊ���ӵ���ջ
/// </summary>
/// <param name="slStack">���ÿ��δ�����ջ</param>
/// <param name="xleft">��ǰ��������߽��</param>
/// <param name="xright">��ǰ�������ұ߽��</param>
/// <param name="y">������y����</param>
/// <param name="bdrColor">�߽���ɫ</param>
/// <param name="newColor">�����ɫ</param>
void SearchLinePoint(stack<PixelPoint>& slStack, int xleft, int xright, int y, unsigned bdrColor, unsigned newColor)
{
	bool span_need_fill = false; // �ж�xleft��xright֮���Ƿ��п��������
	PixelPoint Pt;
	// ����߽�㿪ʼֱ���ұ߽�����
	while (xleft <= xright)
	{
		span_need_fill = false;
		//PixelColor = getPixel(xleft, y);
		while ((getPixel(xleft, y) != bdrColor) && (getPixel(xleft, y) != newColor) && (xleft <= xright))
		{
			span_need_fill = true;
			xleft++;
		}
		if (span_need_fill)
		{
			// ������xright��,��Ϊ�����㣬��˵���ջ,��û�дﵽ����
			if ((getPixel(xleft, y) != bdrColor) && (getPixel(xleft, y) != newColor) && (xleft == xright))
			{
				Pt = { xleft,y };
				slStack.push(Pt);
			}
			else
			{
				Pt = { xleft - 1,y };
				slStack.push(Pt);
			}
		}
		xleft++;
	}
}

void ScanLineAreaFill(int x, int y, unsigned bdrColor, unsigned newColor)
{
	const bool toleft = true;
	const bool toright = false;
	int x0, y0, xright, xleft;
	PixelPoint startPt = { x,y }; // ���ӵ�
	PixelPoint Pt;
	stack<PixelPoint> slStack;// ջ
	slStack.push(startPt);    // ���ӵ���ջ

	// ֱ��ջΪ��
	while (!slStack.empty())
	{
		// ȡ��ջ��Ԫ��
		Pt = slStack.top();
		slStack.pop();

		// ��ջ����x,y��ʼ���Һ������������ɫ��ֱ�������߽�,����������������ұ߽�x����
		x0 = Pt.x;
		y0 = Pt.y;
		auto lrFill = [=](int x, int y, bool direct) {if (direct) { while (getPixel(x, y) != bdrColor) { g_State.drawPixelCB(x, y, newColor); x -= 1; } return x + 1; }
		else
		{
			while (getPixel(x, y) != bdrColor) { g_State.drawPixelCB(x, y, newColor); x += 1; }
			return x - 1;
		}};
		xleft = lrFill(x0, y0, toleft); // �������,�������������߽�x����
		xright = lrFill(x0, y0, toright); // �������,������������ұ߽�x����

		// ����е��������н���ɨ���ҵ����ӵ���ջ
		SearchLinePoint(slStack, xleft, xright, y0 - 1, bdrColor, newColor);
		SearchLinePoint(slStack, xleft, xright, y0 + 1, bdrColor, newColor);
	}//��ջ��
}

double distance(Point2D p1, Point2D p2)
{
	double x2 = (p1.x - p2.x) * (p1.x - p2.x);
	double y2 = (p1.y - p2.y) * (p1.y - p2.y);
	return sqrt(x2 + y2);
}