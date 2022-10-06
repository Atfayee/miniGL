#include "Base3DRenderer.h"
#include "MyRaster.h"
#include "Graphic.h"
#include "Clipper.h"
#include "LightModel.h"

Base3DRender::Base3DRender() 
{
	//ptsConvertToFace(Cube, CubeFace);
}
void Base3DRender::transform_init()
{
	w = getWindowWidth();
	h = getWindowHeight();
	double aspect = w / h;
	mWorld.identityMatrix3D();
	mView.identityMatrix3D();
	mProj = setProjMatrix(PI * 0.5, aspect, 1, 500);
	transform_update();
}

void Base3DRender::transform_update()
{
	mTransform = mWorld * mView * mProj;
}


void Base3DRender::setCameraAtZero(vector3D eye)
{
	vector3D center = { 0,0,0 };
	vector3D up;
	up = { 0,1,0 };
	mView = setViewMatrix(eye, center, up);
	transform_update();
}

void Base3DRender::transform_normalize(vector3D& vec)
{
	double rx = 1 / vec.h;
	vec.x = vec.x * rx  * w * 0.5;
	vec.y = vec.y * rx * h * 0.5;
	vec.z = vec.z * rx;
	vec.h = 1;
}

void Base3DRender::drawTraingle(vector3D& p1, vector3D& p2, vector3D& p3,Color color)
{
	// �޳����������Σ����������ζ���Ϊ˳ʱ������
	if (isCheckBack)
	{
		if (Clipper::check_back(p1, p2, p3, eye) != 0) return;
	}

	// Ӧ�����յı任����
	vector3D c1 = vectorMultiplyMatrix3D(p1, mTransform);
	vector3D c2 = vectorMultiplyMatrix3D(p2, mTransform);
	vector3D c3 = vectorMultiplyMatrix3D(p3, mTransform);

	// �ü���ֱ���޳�����ȫ����׶���ڵ�������,���������Ż�Ϊ�Բ�������׶���ڵ������ν��зָ�
	if (Clipper::check_cvv(c1) != 0) return;
	if (Clipper::check_cvv(c2) != 0) return;
	if (Clipper::check_cvv(c3) != 0) return;

	
	
	// ��һ�����ӿڱ任
	transform_normalize(c1);
	transform_normalize(c2);
	transform_normalize(c3);

	// ����
	if (g_State.g_3DState != Blanking)
	{
		Raster::drawLine(c1.x, c1.y, c2.x, c2.y, BLACK);
		Raster::drawLine(c2.x, c2.y, c3.x, c3.y, BLACK);
		Raster::drawLine(c3.x, c3.y, c1.x, c1.y, BLACK);
		return;
	}
	Raster::Draw3DTriangle(c1, c2, c3, color);
}

void Base3DRender::drawTraingle(texture_v& v1, texture_v& v2, texture_v& v3)
{

	vector3D p1 = v1.pos;
	vector3D p2 = v2.pos;
	vector3D p3 = v3.pos;


	// Ӧ�����յı任����
	vector3D c1 = vectorMultiplyMatrix3D(p1, mTransform);
	vector3D c2 = vectorMultiplyMatrix3D(p2, mTransform);
	vector3D c3 = vectorMultiplyMatrix3D(p3, mTransform);
	double w[3] = { c1.h,c2.h,c3.h };

	// �޳����������Σ����������ζ���Ϊ˳ʱ������
	if (isCheckBack)
	{
		if (Clipper::check_back(p1, p2, p3, eye) != 0) return;
	}


	// �ü���ֱ���޳�����ȫ����׶���ڵ�������,���������Ż�Ϊ�Բ�������׶���ڵ������ν��зָ�
	if (Clipper::check_cvv(c1) != 0) return;
	if (Clipper::check_cvv(c2) != 0) return;
	if (Clipper::check_cvv(c3) != 0) return;



	// ��һ�����ӿڱ任
	transform_normalize(c1);
	transform_normalize(c2);
	transform_normalize(c3);

	// ����
	if (g_State.g_3DState == Blanking) return;
	if (g_State.g_3DState == Texture||g_State.g_3DState == LightGourand)
	{
		texture_v vv1, vv2, vv3;
		vv1.pos = c1;
		vv2.pos = c2;
		vv3.pos = c3;
		// ȡ����Zֵ�����ں�����ֵ
		vv1.pos.h = c1.z;
		vv2.pos.h = c2.z;
		vv3.pos.h = c3.z;
		/*vv1.pos.h = w[0];
		vv2.pos.h = w[1];
		vv3.pos.h = w[2];*/
		vv1.tex = v1.tex; vv2.tex = v2.tex; vv3.tex = v3.tex;
		vv1.RGB = v1.RGB; vv2.RGB = v2.RGB; vv3.RGB = v3.RGB;
		vv1.Light = vv1.Light; vv2.Light = v2.Light; vv3.Light = vv3.Light;
		Raster::Draw3DTriangle(vv1, vv2, vv3);
		return;
	}
	Raster::drawLine(c1.x, c1.y, c2.x, c2.y, BLACK);
	Raster::drawLine(c2.x, c2.y, c3.x, c3.y, BLACK);
	Raster::drawLine(c3.x, c3.y, c1.x, c1.y, BLACK);
}



void Base3DRender::drawTraingle(tpsFace& _cubeFace)
{
	vector3D p1 = _cubeFace[0];
	vector3D p2 = _cubeFace[1];
	vector3D p3 = _cubeFace[2];

	// Ӧ�����յı任����
	vector3D c1 = vectorMultiplyMatrix3D(p1, mTransform);
	vector3D c2 = vectorMultiplyMatrix3D(p2, mTransform);
	vector3D c3 = vectorMultiplyMatrix3D(p3, mTransform);
	// �޳����������Σ����������ζ���Ϊ˳ʱ������
	if (isCheckBack)
	{
		if (Clipper::check_back(p1, p2, p3, eye) != 0) return;
	}


	// �ü���ֱ���޳�����ȫ����׶���ڵ�������,���������Ż�Ϊ�Բ�������׶���ڵ������ν��зָ�
	if (Clipper::check_cvv(c1) != 0) return;
	if (Clipper::check_cvv(c2) != 0) return;
	if (Clipper::check_cvv(c3) != 0) return;



	// ��һ�����ӿڱ任
	transform_normalize(c1);
	transform_normalize(c2);
	transform_normalize(c3);
	_cubeFace.point3D[0] = c1;
	_cubeFace.point3D[1] = c2;
	_cubeFace.point3D[2] = c3;
	
	Lightface lface = getLightFace(_cubeFace);
	int size = 3;
	int miny = -9999;
	int maxy = 9999;
	for (int i = 0; i < size; i++)
	{
		if (_cubeFace[i].y < miny)
			miny = _cubeFace[i].y;
		if (_cubeFace[i].y > maxy)
			maxy = _cubeFace[i].y;
	}
	for (int y = miny; y < maxy; y++)
	{
		//һ�����������洢����ɨ���߽�����Ϣ
		vector<int> gvec;
		//�жϱ���ɨ�����Ƿ������ϵĸ������ཻ
		for (int i = 0; i < size; i++) {
			if (y == int(_cubeFace[i].y)) {
				//ɨ�����ϵ�һ������
				gvec.push_back(_cubeFace[i].x);
			}
			//ɨ�����ڶ���֮��ʱ
			if ((y > int(_cubeFace[i].y) && y < int(_cubeFace[(i + 1) % size].y)) || (y < int(_cubeFace[i].y) && y > int(_cubeFace[(i + 1) % size].y))) {
				int x = _cubeFace[i].x + (y - _cubeFace[i].y) * (_cubeFace[(i + 1) % size].x - _cubeFace[i].x) / (_cubeFace[(i + 1) % size].y - _cubeFace[i].y);
				gvec.push_back(x);
			}
		}
		// ������ɨ�����н����xֵ�����������������������
		sort(gvec.begin(), gvec.end());
		if (gvec.size() < 2)
			continue;
		for (int x = gvec[0]; x < gvec[1]; x++)
		{
			double I;
			double deepz = ToGetDeepthZ(_cubeFace[0], _cubeFace[1], _cubeFace[2], x, y);
			if (g_State.g_3DState == LightSimple)
			{
				I = ToGetLightFaces(x, y, lface, RED,eye);
			}
			if (g_State.g_3DState == LightGourand)
			{

			}
			setPixel(x, y, deepz, _RGB(I * 255, I * 25, I * 80));
		}
	}
}

void Base3DRender::drawPlane(int a, int b, int c, int d,Color color)
{
	if (g_State.g_3DState == Blanking)
	{
		vector3D p1 = Cube[a], p2 = Cube[b], p3 = Cube[c], p4 = Cube[d];
		drawTraingle(p1, p2, p3,color);
		drawTraingle(p3, p4, p1,color);
		return;
	}
	texture_v p1 = CubeNew[a], p2 = CubeNew[b], p3 = CubeNew[c], p4 = CubeNew[d];
	p1.tex.x = 0, p1.tex.y = 0, p2.tex.x = 0, p2.tex.y = texture_grid;
	p3.tex.x = texture_grid, p3.tex.y = texture_grid, p4.tex.x = texture_grid, p4.tex.y = 0;
	drawTraingle(p1, p2, p3);
	drawTraingle(p3, p4, p1);
}

void Base3DRender::displayCube()
{
	
	transform_init();
	setCameraAtZero(eye);
	mWorld = mRotate;
	transform_update();

	mTransform = mTransform * Matrix3D::getMatrixScale(CubeScale);
	mTransform = mTransform * Matrix3D::getMatrixTranslate(dx,dy ,dz);

	ptsConvertToFace(Cube, CubeFace);
	if (g_State.g_3DState == LightSimple)
	{
		for (int i = 0; i < 12; i++)
		{
			drawTraingle(CubeFace[i]);
		}
		return;
	}
	if (g_State.g_3DState == LightGourand)
	{
		//gouraudShading(CubeFace);
		CalLightIntensity(CubeFace);
		//return;
	}

	drawPlane(0, 1, 2, 3, YELLOW);
	drawPlane(4, 5, 6, 7, BLUE);
	drawPlane(0, 4, 5, 1, RED);
	drawPlane(1, 5, 6, 2, GREEN);
	drawPlane(2, 6, 7, 3, _RGB(123, 55, 230));
	drawPlane(3, 7, 4, 0, _RGB(0, 0, 0));
}

void Base3DRender::translate(double dx, double dy, double dz)
{
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
	//transform_update();
}

void Base3DRender::rotate(vector3D axis,double angle)
{
	Matrix3D m = Matrix3D::getMatrixRotate(axis.x, axis.y, axis.z, angle);
	mRotate = mRotate * m;
}

void Base3DRender::Scale(double sX)
{

}

void Base3DRender::eye_Rotate_Update()
{
	vector3D rVec;
	rotateLorE ? rVec = eye : rVec = L;
	if (!isLRtoUD)
	{
		isLRtoUD = true;
		vector3D Yaxis = { 0,1,0 };
		//Xaxis = Yaxis.vector_crossMutiply(eye);
		Xaxis = rVec.vector_crossMutiply(Yaxis);
		return;
	}
}


void Base3DRender::set_eye_Angle(double arc)
{
	Matrix3D eyeRotate = Matrix3D::getMatrixRotate(Xaxis.x, Xaxis.y, Xaxis.z, arc);
	rotateLorE ? eye = vectorMultiplyMatrix3D(eye, eyeRotate) : L = vectorMultiplyMatrix3D(L, eyeRotate);
}

void Base3DRender::set_eye_far(double dis)
{
	if (rotateLorE)
	{
		double scale = (eye.getVectorLength() + dis) / eye.getVectorLength();
		eye = eye * scale;
	}
	else
	{
		double scale = (L.getVectorLength() + dis) / L.getVectorLength();
		L = L * scale;
	}
	
}

vector3D Base3DRender::getEyeCoord()
{
	return eye;
}

void Base3DRender::ptsConvertToFace(vector3D(&pts_)[8], tpsFace(&faces)[12])
{
	vector3D pts[8];
	for (int i = 0; i < 8; i++)
	{
		pts[i] = Cube[i];
	}

	// ǰ
	faces[0].point3D[0] = pts[0];
	faces[0].point3D[1] = pts[1];
	faces[0].point3D[2] = pts[2];
	faces[1].point3D[0] = pts[2];
	faces[1].point3D[1] = pts[3];
	faces[1].point3D[2] = pts[0];
	// ��
	faces[2].point3D[0] = pts[5];
	faces[2].point3D[1] = pts[4];
	faces[2].point3D[2] = pts[7];
	faces[3].point3D[0] = pts[7];
	faces[3].point3D[1] = pts[6];
	faces[3].point3D[2] = pts[5];
	// ��
	faces[4].point3D[0] = pts[0];
	faces[4].point3D[1] = pts[4];
	faces[4].point3D[2] = pts[5];
	faces[5].point3D[0] = pts[5];
	faces[5].point3D[1] = pts[1];
	faces[5].point3D[2] = pts[0];
	// ��
	faces[6].point3D[0] = pts[1];
	faces[6].point3D[1] = pts[5];
	faces[6].point3D[2] = pts[6];
	faces[7].point3D[0] = pts[6];
	faces[7].point3D[1] = pts[2];
	faces[7].point3D[2] = pts[1];
	// ��
	faces[8].point3D[0] = pts[7];
	faces[8].point3D[1] = pts[3];
	faces[8].point3D[2] = pts[2];
	faces[9].point3D[0] = pts[2];
	faces[9].point3D[1] = pts[6];
	faces[9].point3D[2] = pts[7];
	// ��
	faces[10].point3D[0] = pts[4];
	faces[10].point3D[1] = pts[0];
	faces[10].point3D[2] = pts[3];
	faces[11].point3D[0] = pts[3];
	faces[11].point3D[1] = pts[7];
	faces[11].point3D[2] = pts[4];
}



void Base3DRender::setRotateLight(bool isL)
{
	rotateLorE = isL;
}

void Base3DRender::CalLightIntensity(tpsFace(&faces)[12])
{
	// �ȶ�ϵ�����и�ֵ
	Iar = Iag = Iab = 0.5;
	Ipr = Ipg = Ipb = 1;

	Kar = Kag = Kab = 0.1;
	Kdr = Kdg = Kdb = 0.2; // ̫С�ˣ�����һ�������Ŵ�һ��Ч��
	Ksr = Ksg = Ksb = 0.8;

	// �Թ⸳ֵ
	vector3D Light = L;

	//���ȼ���ÿһ����������Ϣ
	int fssize = 12;
	for (int i = 0; i < fssize; ++i)
	{
		groundFace[i].polyID = i;
		groundFace[i].color = i;
		//�������������������Ϣ�洢����Ҫ�о��Ĺ���ģ����ȥ
		//��face[i]�����Ϣ�洢��groundface[i]��

		// �洢���ABCD��Ϣ
		//GetABC(faces[i], groundFace[i].A, groundFace[i].B, groundFace[i].C, groundFace[i].D);

		// �洢��ĵ���Ϣ
		int pointsize = 3;
		for (int j = 0; j < pointsize; ++j)
		{
			groundFace[i].points[j].polyID = i;	 //ÿһ�����������Ķ����
			groundFace[i].points[j].indexID = j; //��ȡ����������е������±�
			groundFace[i].points[j].point = faces[i].point3D[j];
		}

		//�洢��ķ�������Ϣ
		groundFace[i].spmv.x = groundFace[i].A;
		groundFace[i].spmv.y = groundFace[i].B;
		groundFace[i].spmv.z = groundFace[i].C;

		// ʹ�����з�����ָ���������ⲿ
		// ����һ����ͼ������ָ��ͼ����Ķ��������
		vector3D center(0, 0, 0);
		vector3D tempVec = center - groundFace[i].points[0].point;
		if (!SpMVdrector(groundFace[i].spmv, tempVec))
		{
			groundFace[i].spmv.x *= -1;
			groundFace[i].spmv.y *= -1;
			groundFace[i].spmv.z *= -1;
		}

		//�õ�ÿ��������ÿһ����ķ�����
		//�������㴦�ķ�������λ��
		for (int j = 0; j < 3; ++j)
		{
			groundFace[i].points[j].vec = groundFace[i].spmv;
			groundFace[i].points[j].vec.vector_normalize();
		}
	}

	//����ÿһ������ζ����ƽ������ʸ��
	//��ÿ������������ķ�ʽ���д洢����
	vector<vector<PtVector>> SamePoints; //b����ÿһ����
	int ptsize = 8, psize = 3;
	SamePoints.clear();
	SamePoints.resize(ptsize);
	for (int i = 0; i < ptsize; ++i)
	{
		SamePoints[i].clear();
	}
	for (int i = 0; i < ptsize; ++i)
	{
		for (int j = 0; j < fssize; ++j)
		{
			for (int k = 0; k < psize; ++k)
			{
				vector3D tempv = groundFace[j].points[k].point;
				double tx = tempv.x, ty = tempv.y, tz = tempv.z;

				if ((Cube[i].x == tx) && (Cube[i].y == ty) && (Cube[i].z == tz))
				{
					//groundFace[j].points[k].point = vectorMultiplyMatrix3D(groundFace[j].points[k].point, mTransform);
					SamePoints[i].push_back(groundFace[j].points[k]);
				}
			}
		}
	}

	//�������е�ÿһ����������ƽ������
	for (int i = 0; i < SamePoints.size(); ++i)
	{
		vector3D m(0, 0, 0); //����һ���յ�����
		int sizePts = SamePoints[i].size();
		// �ҵ�������ͬλ�õĵ㲢�����ǵķ�����ȡƽ��
		for (int j = 0; j < sizePts; ++j)
		{
			m = m + SamePoints[i][j].vec;
		}
		m.x /= sizePts;
		m.y /= sizePts;
		m.z /= sizePts;
		m.h /= sizePts;

		//�⼸�����ƽ����������������m
		for (int j = 0; j < sizePts; ++j)
		{
			//m.vector_normalize();
			SamePoints[i][j].vec = m;
			SamePoints[i][j].lightcolor = i;
		}
	}

	setRGBv(255, 51, 51);
	for (int i = 0; i < ptsize; i++)
	{
		double x = SamePoints[i][0].point.x;
		double y = SamePoints[i][0].point.y;
		double z = SamePoints[i][0].point.z;
		vector3D norm = SamePoints[i][0].vec;
		double intensity = ToGetGroundFaces(x, y, z, Light, norm, eye,mTransform);
		CubeNew[i].Light = intensity;
		CubeNew[i].RGB.r *= intensity;
		CubeNew[i].RGB.g *= intensity;
		CubeNew[i].RGB.b *= intensity;
	}
	//setRGBv(1, 0.2, 0.2);
}

void Base3DRender::setRGBv(double r, double g, double b)
{
	for (int i = 0; i < 8; i++)
	{
		CubeNew[i].RGB.r = r;
		CubeNew[i].RGB.g = g;
		CubeNew[i].RGB.b = b;
	}
}

void Base3DRender::reset3D()
{
	eye = { 0,0,7 };
	L = { 1,1,1 };
	dx = dy = dz = 0;
	isCheckBack = false;
	texture_grid = 1;
}

