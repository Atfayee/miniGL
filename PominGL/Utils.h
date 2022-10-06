#ifndef _UTILS_H
#define _UTILS_H
#include "MyGeoDefine.h"

struct Layer;
class Utils
{
public:
	static double degreesToRadians(double angle) { return angle * PI / 180.0; }
	static double radiansToDegrees(double angle) { return angle * 180.0 / PI; }
	static Layer* openLayer();
};

#endif
