#ifndef __PHYSRAYCAST_H__
#define __PHYSRAYCAST_H__

#include "MathGeoLib/src/Math/float3.h"
#include "MathGeoLib/src/Geometry/LineSegment.h"

class GameObject;

class PhysRaycast
{
public:
	PhysRaycast();
	~PhysRaycast();

	bool castRay(float3 origin, float3 direction, float distance = std::numeric_limits<float>::infinity());
	bool castRay(const LineSegment& l, float distance = std::numeric_limits<float>::infinity());

public:
	GameObject * gameObject = nullptr;
};

#endif //__PHYSRAYCAST_H__