#pragma once

class FractalImageGenerator
{
public:
	///*c = 0.285, 0.01
	//c = 0.285, 0
	//c = -0.8, 0.156
	//c = -0.835, -0.2321
	//c = -0.70176, -0.3842
	//c = 0.45, -0.1428*/
	static unsigned* generateJuliaSetImage( int width,int height , double c_real, double c_img , double alpha = 1.0 );
	static void generateJuliaSetImage( unsigned* pImage, int width,int height , double c_real = -0.835, double c_img = -0.2321 , double alpha = 1.0 );

	static unsigned* generateMandelbrotImage( int width,int height , double alpha = 1.0 );
	static void generateMandelbrotImage( unsigned* pImage, int width,int height , double alpha = 1.0 );
private:

	static unsigned getColor( int level , double alpha );
	static void HSV2RGB(float H, float S, float V, float &R, float &G, float &B);	
};

