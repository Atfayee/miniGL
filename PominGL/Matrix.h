#pragma once
#include "Graphic.h"
#include "Vec.h"
#include "MyGeoDefine.h"
#include "DrawState.h"
#include <math.h>

/// <summary>
/// 用于二维变换的矩阵类
/// </summary>
class Matrix2D
{
public:
	double matrix[3][3];  // 定义一个3×3的数组
	static string TransformType;

	// 0->平移;1->旋转;2->缩放;3->对称;4->错切;
	int transformType;
	// 构造函数
	Matrix2D()
	{
		// 构造函数，初始化矩阵为单位矩阵
		InitMatrix();
	}
	Matrix2D(double matrixNew[3][3])
	{
		// 构造函数的重载，将数组转换为矩阵
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i == j)
					matrix[i][j] = matrixNew[i][j];
				else
					matrix[i][j] = matrixNew[i][j];
			}
		}
	}
	// 矩阵初始化
	void InitMatrix()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (i == j) matrix[i][j] = 1;
				else matrix[i][j] = 0;
	}
	//重载运算符“+”，矩阵相加
	Matrix2D operator+(const Matrix2D& b)
	{
		Matrix2D result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result.matrix[i][j] = matrix[i][j] + b.matrix[i][j];
		return result;
	}
	//重载运算符“-”，矩阵相减
	Matrix2D operator-(const Matrix2D& b)
	{
		Matrix2D result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result.matrix[i][j] = matrix[i][j] - b.matrix[i][j];
		return result;
	}
	//重载运算符“*”，矩阵相乘
	Matrix2D operator*(const Matrix2D& b)
	{
		Matrix2D result;
		double sum = 0.0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				sum = 0.0;
				for (int k = 0; k < 3; k++)
					sum += matrix[i][k] * b.matrix[k][j];
				result.matrix[i][j] = sum;
			}
		return result;
	}
	// 重载运算符“*=”，矩阵相乘
	Matrix2D operator*=(const Matrix2D& b)
	{
		//Matrix2D result;
		//Matrix2D ThisMatrix(matrix);
		*this = *this * b;
		return *this;
	}

	//平移变换矩阵
	Matrix2D static Translate(double Tx, double Ty)
	{
		//TransformType = "pan";
		Matrix2D NewMatrix;
		NewMatrix.matrix[2][0] = Tx;
		NewMatrix.matrix[2][1] = Ty;
		NewMatrix.transformType = 0;
		return NewMatrix;
	}
	//缩放变换矩阵
	Matrix2D static Scale(double S)
	{
		TransformType = "scale";
		Matrix2D NewMatrix;
		NewMatrix.matrix[0][0] *= S;
		NewMatrix.matrix[1][1] *= S;
		NewMatrix.transformType = 2;
		return NewMatrix;
	}
	//旋转变换矩阵
	Matrix2D static Rotate(double Angle)
	{
		//TransformType = "rotate";
		Matrix2D NewMatrix;
		NewMatrix.matrix[0][0] = cos(Angle / 180 * PI);
		NewMatrix.matrix[0][1] = sin(Angle / 180 * PI);
		NewMatrix.matrix[1][0] = -sin(Angle / 180 * PI);
		NewMatrix.matrix[1][1] = cos(Angle / 180 * PI);
		NewMatrix.transformType = 1;
		return NewMatrix;
	}
	// 对称变换矩阵
	Matrix2D static Symmetry(SymmetryType syType)
	{
		Matrix2D NewMatrix;
		switch (syType)
		{
		case symmetryOrigin:
			NewMatrix.matrix[0][0] = NewMatrix.matrix[1][1] = -1;
			break;
		case symmetryX:
			NewMatrix.matrix[1][1] = -1;
			break;
		case symmetryY:
			NewMatrix.matrix[0][0] = -1;
			break;
		case symmetryXY:
			NewMatrix.matrix[0][0] = NewMatrix.matrix[1][1] = 0;
			NewMatrix.matrix[0][1] = NewMatrix.matrix[1][0] = 1;
			break;
		case symmetryYX:
			NewMatrix.matrix[0][0] = NewMatrix.matrix[1][1] = 0;
			NewMatrix.matrix[0][1] = NewMatrix.matrix[1][0] = -1;
			break;
		}
		NewMatrix.transformType = 3;
		return NewMatrix;
	}

	// 错切变换矩阵
	Matrix2D static Shear(double dx, double dy)
	{
		Matrix2D NewMatrix;
		NewMatrix.matrix[1][0] = dx;
		NewMatrix.matrix[0][1] = dy;
		NewMatrix.transformType = 4;
		return NewMatrix;
	}
};

//string Matrix2D::TransformType = "none";

// 变换矩阵与齐次坐标表示向量相乘
vector2D Matrix2D_Multiply(const vector2D& point, Matrix2D& newmatrix);

// 三维坐标齐次向量表示
class Matrix3D
{
public:
	double matrix[4][4];
	Matrix3D()
	{
		identityMatrix3D();
	}
	~Matrix3D(){}
	void identityMatrix3D()
	{
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
					matrix[i][j] = 1;
				else
					matrix[i][j] = 0;
			}
	}

	Matrix3D operator*(const Matrix3D& m);
	Matrix3D operator*=(const Matrix3D& m);
	Matrix3D operator*=(const double& f);
	double* operator[](int row);

	void setZero();
	// 平移矩阵
	static Matrix3D getMatrixTranslate(double dx, double dy, double dz);
	// 整体缩放矩阵
	static Matrix3D getMatrixScale(double dt);
	// 不同方向缩放矩阵
	static Matrix3D getMatrixScale(double dx, double dy, double dz);
	// 旋转矩阵
	static Matrix3D getMatrixRotate(double x, double y, double z, double Angle);
};
// 观察坐标系变换矩阵
Matrix3D setViewMatrix(vector3D& eye,vector3D& center, vector3D& up);

// 投影变换矩阵
Matrix3D setProjMatrix(double fovy, double aspect, double zn, double zf);
// 坐标变换，向量与矩阵相乘
vector3D vectorMultiplyMatrix3D(vector3D& vec, Matrix3D& m);
