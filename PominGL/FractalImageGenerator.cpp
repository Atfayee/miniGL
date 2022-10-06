
#include "FractalImageGenerator.h"
#include "Graphic.h"

struct Complex
{
	double real, img;
};

unsigned FractalImageGenerator::getColor(int level, double alpha )
{
	float h = level * 9 % 360;
	float s = 0.7 ;
	float v = (1.0 - level / 360.0 * level / 360.0 / 1.2);
	float R,G,B;
	HSV2RGB( h, s, v ,R,G,B );
	return _RGBA( R * 255, G * 255, B * 255, alpha * 255 );
}

void FractalImageGenerator::HSV2RGB(float H, float S, float V, float &R, float &G, float &B)
{
	int i;
	float f, p, q, t;

	if( S == 0 ) 
	{
		R = G = B = V;
		return;
	}

	H /= 60; // sector 0 to 5
	i = (int) H ;
	f = H - i; // factorial part of h
	p = V * ( 1 - S );
	q = V * ( 1 - S * f );
	t = V * ( 1 - S * ( 1 - f ) );

	switch( i )
	{
	case 0: 
		R = V;
		G = t;
		B = p;
		break;
	case 1:
		R = q;
		G = V;
		B = p;
		break;
	case 2:
		R = p;
		G = V;
		B = t;
		break;
	case 3:
		R = p;
		G = q;
		B = V;
		break;
	case 4:
		R = t;
		G = p;
		B = V;
		break;
	default: // case 5:
		R = V;
		G = p;
		B = q;
		break;
	}
}

unsigned* FractalImageGenerator::generateJuliaSetImage( int width,int height , double c_real, double c_img , double alpha )
{
	unsigned* pImage = new unsigned[width * height * sizeof(unsigned)];
	generateJuliaSetImage( pImage, width, height, c_real, c_img  , alpha );
	return pImage;
}

void FractalImageGenerator::generateJuliaSetImage(unsigned* pImage, int width,int height , double c_real, double c_img, double alpha )
{
	double XMax = 2.5;//复平面的最大坐标
	double XMin = -2.5;
	double YMax = 2.5;
	double YMin = -2.5;

	double deltaX = (XMax - XMin) / width;
	double deltaY = (YMax - YMin) / height;
	int max_iterations = 360;//最大迭代次数
	double max_size = 4.0;//

	for(int row = 0;row < height;row++)
	{
		for(int col = 0;col < width;col++)
		{
			int level = 0;
			Complex c,z;
			//z.real = 0;//这里是Mandelbrot集，下面被注释掉的是Julia集
			//z.img = 0;
			//c.real  = XMin + col * deltaX;
			//c.img = YMin + row * deltaY;
			z.real = XMin + col * deltaX;
			z.img = YMin + row * deltaY;

			c.real = c_real;
			c.img = c_img;

			while((level < max_iterations) && ((z.img * z.img + z.real * z.real) < max_size))
			{	
				double tmp = z.real * z.real - z.img * z.img + c.real;
				z.img = z.img * z.real + z.real * z.img + c.img;
				z.real = tmp;
				level++;
			}

			*pImage++ = getColor( 255 - level , alpha );
		}
	}
}

unsigned* FractalImageGenerator::generateMandelbrotImage(int width,int height , double alpha )
{
	unsigned* pImage = new unsigned[width * height * sizeof(unsigned)];
	generateMandelbrotImage( pImage, width, height  , alpha );
	return pImage;
}

void FractalImageGenerator::generateMandelbrotImage(unsigned* pImage, int width,int height , double alpha )
{
	double XMax = 2.5;//复平面的最大坐标
	double XMin = -2.5;
	double YMax = 2.5;
	double YMin = -2.5;

	double deltaX = (XMax - XMin) / width;
	double deltaY = (YMax - YMin) / height;
	int max_iterations = 360;//最大迭代次数
	double max_size = 4.0;//

	for(int row = 0;row < height;row++)
	{
		for(int col = 0;col < width;col++)
		{
			int level = 0;
			Complex c,z;
			z.real = 0;
			z.img = 0;
			c.real  = XMin + col * deltaX;
			c.img = YMin + row * deltaY;

			while((level < max_iterations) && ((z.img * z.img + z.real * z.real) < max_size))
			{	
				double tmp = z.real * z.real - z.img * z.img + c.real;
				z.img = z.img * z.real + z.real * z.img + c.img;
				z.real = tmp;
				level++;
			}

			*pImage++ = getColor( 255 - level , alpha );
		}
	}
}
