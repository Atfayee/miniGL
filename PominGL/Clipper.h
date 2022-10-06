#pragma once
#include "Vec.h"
#include "LightModel.h"

class Clipper
{
public:
	/// <summary>
	/// 检查顶点坐标，判断是否在视锥体边界
	/// </summary>
	/// <param name="v">顶点</param>
	/// <returns>在边界内返回0</returns>
	static int check_cvv(vector3D& v)
	{
		double w = v.h;
		int check = 0;
		if (v.z < 0.0) check |= 1;
		if (v.z > w) check |= 2;
		if (v.x < -w) check |= 4;
		if (v.x > w) check |= 8;
		if (v.y < -w) check |= 16;
		if (v.y > w) check |= 32;
		return check;
	}
	/// <summary>
	/// 剔除背面三角形
	/// </summary>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="eye"></param>
	/// <returns></returns>
	static int check_back(vector3D& p1, vector3D& p2, vector3D& p3,vector3D& eye)
	{
		vector3D c1 = p1 - p2;
		vector3D c2 = p3 - p2;
		vector3D fa = c2.vector_crossMutiply(c1);
		vector3D center(0, 0, 0);
		vector3D temp = center - p1;
		if (!SpMVdrector(fa, temp))
		{
			fa = fa*(-1);
		}
		vector3D eyeT = p3 - eye;
		double cosa = eyeT.vector_dotMutiply(fa);
		if (cosa > 0) return 0;
		return 1;
	}
};
