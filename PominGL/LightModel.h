#pragma once
#include "Graphic.h"
#include "Vec.h"
#include "Base3DRenderer.h"
#include "Matrix.h"


//=======================================�ֲ�����ģ�Ͳ���=========================================

	// ����㶨��ǿ�µ�ÿ����������Ľṹ��
struct Lightface {
	int polyID;   // ���
	double A, B, C, D;  //�淽��x+By+Cz+D=0��ϵ��
	vector3D spmv;   //��ķ�����
	Color color;   //�����ɫ
	vector3D points[3]; //�������������������洢
};

// newBuffer�����洢����ģ����ÿ�����ص���ɫֵ
vector<vector<Color>> newBuffer;

// rgb����ģ�Ͳ���
// 
// ������ ambient light
double Iar = 0.2, Iag = 0.2, Iab = 0.2;
// �����Ip
double Ipr = 0.5, Ipg = 0.5, Ipb = 0.5;

// �����ⷴ��ϵ�� Ienv=KaIa
double Kar = 0.1, Kag = 0.1, Kab = 0.1;

// ������ diffuse reflection
// Lambert������ģ��
// n��IΪ����Ƕ�
// rΪ��Դ�������ľ���
// Ld=Kd(I/r*r)max(0,n��I)
// ������ϵ�� Kd 
double Kdr = (0.8 * 300), Kdg = (0.8 * 300), Kdb = (0.8 * 300);

// ���淴��� specular hightlights
// ���淴��ϵ�� Ks
// ���ǹ۲��ߵ��ӽ�
// Ls=Ks(I/r*r)max(0,cos��)P
double Ksr=0.5, Ksg=0.5, Ksb=0.5;

//============================================================================================
//=========================����¹���ģ�Ͳ���======================================
struct rgb
{
	double r, g, b;
};
struct PtVector
{
	int polyID;   //�ö��������Ķ����
	int indexID;  //����������е�˳�������
	vector3D point;
	vector3D vec;
	double light;   //��������Ĺ���ǿ��
	rgb RGB;
	int lightcolor;
};

struct GrdFaces {
	int polyID;
	double A, B, C, D;
	vector3D spmv;
	Color color;  //ÿ�������ɫ
	PtVector points[3];   //һ�������������
	int size = 3;  //������
};
GrdFaces groundFace[12];

double GetDistance(vector3D& Light, vector3D& Targrt)
{
	double d1 = sqrt(pow(Targrt.x - Light.x, 2) + pow(Targrt.y - Light.y, 2) + pow(Targrt.z - Light.z, 2));
	//double d2 = sqrt(pow(Targrt.x - eye.x, 2) + pow(Targrt.y - eye.y, 2) + pow(Targrt.z - eye.z, 2));
	return d1;
}
vector3D getReflect(vector3D light, vector3D norm)
{

	//���㹫ʽ��R=I-2(I��N)N

	vector3D reflect;
	double K = 2 * light.vector_dotMutiply(norm);

	vector3D temp;
	temp.x = K * norm.x;
	temp.y = K * norm.y;
	temp.z = K * norm.z;
	temp.h = K * norm.h;

	reflect = light - temp;
	return reflect;
}

void GetABC(Base3DRender::tpsFace& face, double& a, double& b, double& c, double& d)
{

	vector3D pt0, pt1, pt2;
	pt0 = face.point3D[0];
	pt1 = face.point3D[1];
	pt2 = face.point3D[2];
	double mx = pt1.x - pt0.x;
	double my = pt1.y - pt0.y;
	double mz = pt1.z - pt0.z;

	double nx = pt2.x - pt0.x;
	double ny = pt2.y - pt0.y;
	double nz = pt2.z - pt0.z;
	a = my * nz - ny * mz;
	b = nx * mz - mx * nz;
	c = mx * ny - nx * my;

	d = -(a * pt0.x + b * pt0.y + c * pt0.z);
}
bool SpMVdrector(vector3D m, vector3D n)
{
	//�ж�2�����������Ƿ�ͬ��
	//ͬ�� ����Ϊtrue ���� ����Ϊfalse
	if ((m.x * n.x + m.y * n.y + m.z * n.z) > 0)
		return true;
	return false;
}
Lightface getLightFace(Base3DRender::tpsFace& faces)
{

	Lightface newLightface;
	/*newLightface.polyID = i;
	newLightface.color = i;*/
	//�������������������Ϣ�洢����Ҫ�о��Ĺ���ģ����ȥ
	//��face[i]�����Ϣ�洢��Lightface[i]��

	// �洢���ABCD��Ϣ
	GetABC(faces, newLightface.A, newLightface.B, newLightface.C, newLightface.D);
	// �洢��ĵ���Ϣ
	int pointsize = 3;
	for (int j = 0; j < pointsize; j++)
	{
		newLightface.points[j] = faces.point3D[j];
	}
	// �洢��ķ�������Ϣ
	newLightface.spmv.x = newLightface.A;
	newLightface.spmv.y = newLightface.B;
	newLightface.spmv.z = newLightface.C;

	// ����һ����ͼ������ָ��ͼ����Ķ��������
	vector3D center(0, 0, 0);
	vector3D tempVec = center - newLightface.points[0];

	// ʹ�����з�����ָ���������ⲿ
	if (!SpMVdrector(newLightface.spmv, tempVec))
	{
		newLightface.spmv.x *= -1;
		newLightface.spmv.y *= -1;
		newLightface.spmv.z *= -1;
	}
	return newLightface;
}

double GetZdeep(double A, double B, double C, double D, double x, double y)
{
	if (C == 0)
	{
		return -9999;
	}

	//�������
	return (A * x + B * y + D) / (-C);
}


double ToGetLightFaces(double x, double y, Lightface& lface, Color gcolor,vector3D& eye)
{
	Iar = Iag = Iab = 0.2;
	Ipr = Ipg = Ipb = 0.5;

	Kar = Kag = Kab = 0.1;
	Kdr = Kdg = Kdb = 0.8 * 300; // ̫С�ˣ�����һ�������Ŵ�һ��Ч��
	Ksr = Ksg = Ksb = 0.5;

	// �Թ⸳ֵ
	//vector3D Light(0, 0, 1);
	vector3D Light = L;
	double Ia, Id, Is;

	// 1��������
	Ia = Kar * Iar;

	// 2��������
	// ������
	vector3D norm = lface.spmv;
	norm.vector_normalize();

	double z = GetZdeep(lface.A, lface.B, lface.C, lface.D, x, y);
	// �����λ��
	vector3D fragPos(x, y, z);
	// ���߷���
	// LightΪ��Դ����
	vector3D lightDir = Light - fragPos;
	lightDir.vector_normalize();

	float diff = norm.vector_dotMutiply(lightDir) > 0 ? norm.vector_dotMutiply(lightDir) : 0;
	// ������Ӧ�û�Ҫ��һ���������ϵ��r*r������Ч����̫��
	/*double distance = sqrt(pow(norm.x - fragPos.x, 2) + pow(norm.y - fragPos.y, 2));
	double r = distance > 1 ? distance : 1;*/
	Id = diff * Kdr * Ipr;

	// 3�����淴��
	//vector3D viewPos(260, 220, -3.5);
	vector3D viewPos = eye * 10;
	vector3D viewDir = viewPos - fragPos;
	viewDir.vector_normalize();
	vector3D reflectDir = getReflect(lightDir, norm);
	double dot = viewDir.vector_dotMutiply(reflectDir) > 0 ? viewDir.vector_dotMutiply(reflectDir) : 0;
	// �߹�ķ����(Shininess)
	//һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС
	int shininess = 3;
	float spec = pow(dot, shininess);
	double specLight = 1;
	Is = spec * Ksr * specLight;

	double I = Id + Ia + Is;
	return I;
}

GrdFaces getGrdFace(Base3DRender::tpsFace& face)
{
	GrdFaces groundFace;

	//�������������������Ϣ�洢����Ҫ�о��Ĺ���ģ����ȥ
	//��face[i]�����Ϣ�洢��groundface[i]��

	// �洢���ABCD��Ϣ
	GetABC(face, groundFace.A, groundFace.B, groundFace.C, groundFace.D);

	// �洢��ĵ���Ϣ
	int pointsize = 3;
	for (int j = 0; j < pointsize; ++j)
	{
		//groundFace.points[j].polyID = i;	 //ÿһ�����������Ķ����
		groundFace.points[j].indexID = j; //��ȡ����������е������±�
		groundFace.points[j].point = face.point3D[j];
	}

	//�洢��ķ�������Ϣ
	groundFace.spmv.x = groundFace.A;
	groundFace.spmv.y = groundFace.B;
	groundFace.spmv.z = groundFace.C;

	// ʹ�����з�����ָ���������ⲿ
	// ����һ����ͼ������ָ��ͼ����Ķ��������
	vector3D center(0, 0, 0);
	vector3D tempVec = center - groundFace.points[0].point;
	if (!SpMVdrector(groundFace.spmv, tempVec))
	{
		groundFace.spmv.x *= -1;
		groundFace.spmv.y *= -1;
		groundFace.spmv.z *= -1;
	}

	//�õ�ÿ��������ÿһ����ķ�����
	//�������㴦�ķ�������λ��
	for (int j = 0; j < 3; ++j)
	{
		groundFace.points[j].vec = groundFace.spmv;
		groundFace.points[j].vec.vector_normalize();
	}
	return groundFace;
}


double ToGetGroundFaces(double x, double y, Color pixel, Color ColorMode, vector3D Light, vector3D& tarv,vector3D& eye)
{
	Kdr = Kdg = Kdb = 0.8 * 300;
	double Ia, Id, Is;
	// 1��������
	Ia = Kar * Iar;
	// 2��������
	// ������
	vector3D norm = tarv;
	//norm.vector_normalize();

	//�����Դ����������������
	//�����������x,y����ά�ռ�����
	double z = GetZdeep(groundFace[pixel].A, groundFace[pixel].B, groundFace[pixel].C, groundFace[pixel].D, x, y);
	// �����λ��
	vector3D fragPos(x, y, z);

	// ���߷���
	// LightΪ��Դ����
	vector3D lightDir = Light - fragPos;
	lightDir.vector_normalize();
	float diff = norm.vector_dotMutiply(lightDir) > 0 ? norm.vector_dotMutiply(lightDir) : 0;
	// ������Ӧ�û�Ҫ��һ���������ϵ��r*r������Ч����̫��
	/*double distance = sqrt(pow(norm.x - fragPos.x, 2) + pow(norm.y - fragPos.y, 2));
	double r = distance > 1 ? distance : 1;*/
	Id = diff * Kdr * Ipr;
	// 3�����淴��
	//vector3D viewPos(260, 220, -0.5);
	vector3D viewPos = eye * 10;
	vector3D viewDir = viewPos - fragPos;
	viewDir.vector_normalize();
	vector3D reflectDir = getReflect(lightDir, norm);
	double dot = viewDir.vector_dotMutiply(reflectDir) > 0 ? viewDir.vector_dotMutiply(reflectDir) : 0;
	// �߹�ķ����(Shininess)
	//һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС
	int shininess = 3;
	float spec = pow(dot, shininess);
	double specLight = 1;
	Is = spec * Ksr * specLight;

	/*double c0 = 0.8;
	double c1 = 0.0000001;
	double c2 = 0.000000001;
	double distance = GetDistance(Light, fragPos) + GetDistance(fragPos,viewPos);
	double disDecay = 1 / (c0 + c1 * distance + c2 * pow(distance, 2));
	double fd = min(1.0, distance);
	Id *= fd;
	Is *= fd;*/

	double I = Id + Ia + Is;
	return I;
}


double ToGetGroundFaces(double x, double y,  double z,  vector3D Light, vector3D& tarv, vector3D& eye,Matrix3D& m)
{
	Kdr = Kdg = Kdb = 0.2;
	double Ia, Id, Is;
	// 1��������
	Ia = Kar * Iar;
	// 2��������
	// ������
	vector3D norm = tarv;
	norm.vector_normalize();
	// �����λ��
	vector3D fragPos(x, y, z);
	fragPos = vectorMultiplyMatrix3D(fragPos, m);
	//fragPos = fragPos * (1.0 / fragPos.h);

	// ���߷���
	// LightΪ��Դ����
	vector3D lightDir = Light - fragPos;
	lightDir.vector_normalize();
	float diff = lightDir.vector_dotMutiply(norm) > 0 ? lightDir.vector_dotMutiply(norm) : 0;
	// ������Ӧ�û�Ҫ��һ���������ϵ��r*r������Ч����̫��
	/*double distance = sqrt(pow(norm.x - fragPos.x, 2) + pow(norm.y - fragPos.y, 2));
	double r = distance > 1 ? distance : 1;*/
	Id = diff * Kdr * Ipr;
	// 3�����淴��
	//vector3D viewPos(260, 220, -0.5);
	vector3D viewPos = eye;
	vector3D viewDir = viewPos - fragPos;
	viewDir.vector_normalize();
	vector3D reflectDir = getReflect(lightDir*(-1), norm);
	double dot = reflectDir.vector_dotMutiply(viewDir) > 0 ? reflectDir.vector_dotMutiply(viewDir) : 0;
	// �߹�ķ����(Shininess)
	//һ������ķ����Խ�ߣ�����������Խǿ��ɢ���Խ�٣��߹��ͻ�ԽС
	vector3D H = (lightDir + viewDir) * 0.5;
	H.vector_normalize();
	int shininess = 1;
	float spec = sqrt(pow(dot, shininess));
	//Is = spec * Ksr * specLight;
	Is = spec * Ksr * Ipr;

	double c0 = 0.8;
	double c1 = 0.0000001;
	double c2 = 0.000000001;
	double distance = lightDir.getVectorLength()+viewDir.getVectorLength();
	double disDecay = 1 / (c0 + c1 * distance + c2 * pow(distance, 2));
	double fd = min(1.0, disDecay);
	Id *= fd;
	Is *= fd;

	double I = Id + Ia + Is;
	return I;
}











