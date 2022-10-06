#include "Matrix.h"

vector2D Matrix2D_Multiply(const vector2D& point, Matrix2D& newmatrix)
{
	vector2D newpoint;
	newpoint.x = point.x * newmatrix.matrix[0][0] + point.y * newmatrix.matrix[1][0] + point.h * newmatrix.matrix[2][0];
	newpoint.y = point.x * newmatrix.matrix[0][1] + point.y * newmatrix.matrix[1][1] + point.h * newmatrix.matrix[2][1];
	newpoint.h = point.x * newmatrix.matrix[0][2] + point.y * newmatrix.matrix[1][2] + point.h * newmatrix.matrix[2][2];
	return newpoint;
}

Matrix3D Matrix3D::operator*(const Matrix3D& m)
{
	Matrix3D NewMatrix3D;
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			NewMatrix3D.matrix[i][j] = 0;
			for (int k = 0; k < 4; k++)
				NewMatrix3D.matrix[i][j] += matrix[i][k] * m.matrix[k][j];
		}
	return NewMatrix3D;
}

Matrix3D Matrix3D::operator*=(const Matrix3D& m)
{
	return (*this) * m;
}

Matrix3D Matrix3D::operator*=(const double& f)
{
	Matrix3D NewMatrix3D;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			NewMatrix3D.matrix[i][j] *= f;
	return NewMatrix3D;
}
double* Matrix3D::operator[](int row)
{
	return matrix[row];
}

void Matrix3D::setZero()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix[i][j] = 0;
}


Matrix3D Matrix3D::getMatrixTranslate(double dx, double dy, double dz)
{
	Matrix3D NewResult;
	NewResult[3][0] = dx;
	NewResult[3][1] = dy;
	NewResult[3][2] = dz;
	return NewResult;
}

Matrix3D Matrix3D::getMatrixScale(double dt)
{
	Matrix3D NewResult;
	NewResult[3][3] = dt;
	return NewResult;
}

Matrix3D Matrix3D::getMatrixScale(double dx, double dy, double dz)
{
	Matrix3D NewResult;
	NewResult[0][0] = dx;
	NewResult[1][1] = dy;
	NewResult[2][2] = dz;
	return NewResult;
}

Matrix3D Matrix3D::getMatrixRotate(double x, double y, double z, double Angle)
{
	Matrix3D NewResult;
	double qsin = sin(Angle * 0.5);
	double qcos = cos(Angle * 0.5);
	vector3D vec = { x, y, z };
	vec.vector_normalize();
	double w = qcos;
	x = vec.x * qsin;
	y = vec.y * qsin;
	z = vec.z * qsin;
	NewResult[0][0] = 1 - 2 * y * y - 2 * z * z;
	NewResult[1][0] = 2 * x * y - 2 * w * z;
	NewResult[2][0] = 2 * x * z + 2 * w * y;
	NewResult[0][1] = 2 * x * y + 2 * w * z;
	NewResult[1][1] = 1 - 2 * x * x - 2 * z * z;
    NewResult[2][1] = 2 * y * z - 2 * w * x;
	NewResult[0][2] = 2 * x * z - 2 * w * y;
	NewResult[1][2] = 2 * y * z + 2 * w * x;
	NewResult[2][2] = 1 - 2 * x * x - 2 * y * y;
	return NewResult;
}

Matrix3D setViewMatrix(vector3D& eye, vector3D& center, vector3D& up)
{
	Matrix3D result;
	vector3D xaxis, yaxis, zaxis;
	zaxis = center - eye;
	zaxis.vector_normalize();
	xaxis = up.vector_crossMutiply(zaxis);
	xaxis.vector_normalize();
	yaxis = zaxis.vector_crossMutiply(xaxis);
	//yaxis.vector_normalize();

	result[0][0] = xaxis.x;
	result[1][0] = xaxis.y;
	result[2][0] = xaxis.z;
	result[3][0] = -xaxis.vector_dotMutiply(eye);

	result[0][1] = yaxis.x;
	result[1][1] = yaxis.y;
	result[2][1] = yaxis.z;
	result[3][1] = -yaxis.vector_dotMutiply(eye);

	result[0][2] = zaxis.x;
	result[1][2] = zaxis.y;
	result[2][2] = zaxis.z;
	result[3][2] = -zaxis.vector_dotMutiply(eye);
	return result;
}

Matrix3D setProjMatrix(double fovy, double aspect, double zn, double zf)
{
	Matrix3D projM;
	projM.setZero();
	double fax = 1.0 / tan(fovy * 0.5);
	projM[0][0] = fax / aspect;
	projM[1][1] = fax;
	projM[2][2] = zf / (zf - zn);
	projM[3][2] = -zn * zf / (zf - zn);
	projM[2][3] = 1;
	return projM;
}

vector3D vectorMultiplyMatrix3D(vector3D& vec, Matrix3D& m)
{
	vector3D result;
	
	result.x = vec.x * m[0][0] + vec.y * m[1][0] + vec.z * m[2][0] + vec.h * m[3][0];
	result.y = vec.x * m[0][1] + vec.y * m[1][1] + vec.z * m[2][1] + vec.h * m[3][1];
	result.z = vec.x * m[0][2] + vec.y * m[1][2] + vec.z * m[2][2] + vec.h * m[3][2];
	result.h = vec.x * m[0][3] + vec.y * m[1][3] + vec.z * m[2][3] + vec.h * m[3][3];
	
	return result;
}



