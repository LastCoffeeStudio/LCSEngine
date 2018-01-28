#include <cmath>
#include "MathGeoLib\src\MathGeoLib.h"

float distance3(float3 a, float3 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

float distance2(float2 a, float2 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow(a.y - b.y, 2));
}