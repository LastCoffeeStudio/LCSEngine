#ifndef __BILLBOARD_H__
#define __BILLBOARD_H__

#include "MathGeoLib/src/Geometry/Frustum.h"
#include "MathGeoLib/src/Math/float3.h"

class Billboard
{
public:
	Billboard(float3 position, float width, float height);
	~Billboard();

	void ComputeQuad(const Frustum* frustum, std::vector<float3>& vertexs);

public:
	float3 position = float3::zero;
	float width = 0.f;
	float height = 0.f;
};

#endif //__BILLBOARD_H__