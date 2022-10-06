#pragma once
#include "Graphic.h"
#include "Vec.h"
#include "MyGeoDefine.h"
#include "DrawState.h"
#include <math.h>

/// <summary>
/// ���ڶ�ά�任�ľ�����
/// </summary>
class Matrix2D
{
public:
	double matrix[3][3];  // ����һ��3��3������
	static string TransformType;

	// 0->ƽ��;1->��ת;2->����;3->�Գ�;4->����;
	int transformType;
	// ���캯��
	Matrix2D()
	{
		// ���캯������ʼ������Ϊ��λ����
		InitMatrix();
	}
	Matrix2D(double matrixNew[3][3])
	{
		// ���캯�������أ�������ת��Ϊ����
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
	// �����ʼ��
	void InitMatrix()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				if (i == j) matrix[i][j] = 1;
				else matrix[i][j] = 0;
	}
	//�����������+�����������
	Matrix2D operator+(const Matrix2D& b)
	{
		Matrix2D result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result.matrix[i][j] = matrix[i][j] + b.matrix[i][j];
		return result;
	}
	//�����������-�����������
	Matrix2D operator-(const Matrix2D& b)
	{
		Matrix2D result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result.matrix[i][j] = matrix[i][j] - b.matrix[i][j];
		return result;
	}
	//�����������*�����������
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
	// �����������*=�����������
	Matrix2D operator*=(const Matrix2D& b)
	{
		//Matrix2D result;
		//Matrix2D ThisMatrix(matrix);
		*this = *this * b;
		return *this;
	}

	//ƽ�Ʊ任����
	Matrix2D static Translate(double Tx, double Ty)
	{
		//TransformType = "pan";
		Matrix2D NewMatrix;
		NewMatrix.matrix[2][0] = Tx;
		NewMatrix.matrix[2][1] = Ty;
		NewMatrix.transformType = 0;
		return NewMatrix;
	}
	//���ű任����
	Matrix2D static Scale(double S)
	{
		TransformType = "scale";
		Matrix2D NewMatrix;
		NewMatrix.matrix[0][0] *= S;
		NewMatrix.matrix[1][1] *= S;
		NewMatrix.transformType = 2;
		return NewMatrix;
	}
	//��ת�任����
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
	// �ԳƱ任����
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

	// ���б任����
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

// �任��������������ʾ�������
vector2D Matrix2D_Multiply(const vector2D& point, Matrix2D& newmatrix);

// ��ά�������������ʾ
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
	// ƽ�ƾ���
	static Matrix3D getMatrixTranslate(double dx, double dy, double dz);
	// �������ž���
	static Matrix3D getMatrixScale(double dt);
	// ��ͬ�������ž���
	static Matrix3D getMatrixScale(double dx, double dy, double dz);
	// ��ת����
	static Matrix3D getMatrixRotate(double x, double y, double z, double Angle);
};
// �۲�����ϵ�任����
Matrix3D setViewMatrix(vector3D& eye,vector3D& center, vector3D& up);

// ͶӰ�任����
Matrix3D setProjMatrix(double fovy, double aspect, double zn, double zf);
// ����任��������������
vector3D vectorMultiplyMatrix3D(vector3D& vec, Matrix3D& m);
