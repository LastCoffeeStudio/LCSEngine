#include "Billboard.h"

Billboard::Billboard() {}
Billboard::Billboard(float3 position, float width, float height) : position(position), width(width), height(height) {}

Billboard::~Billboard() {}

/*Vertexs will contain all 4 vertexs of the quad in this order: Up Left, Up Right, Down Left, Down Right*/
void Billboard::ComputeQuad(const Frustum* frustum, std::vector<float3>& vertexs)
{
	float3 right = (position - frustum->pos).Cross(float3(0.f,1.f,0.f)).Normalized();
	
	float3 h = (float3(0.f, 1.f, 0.f) * height);
	float3 w = (right * width);

	vertexs.clear();
	vertexs.reserve(4);
	vertexs.push_back(position + (float3(0.f, 1.f, 0.f) * height) - (right * width) / 2.f);
	vertexs.push_back(position + (float3(0.f, 1.f, 0.f) * height) + (right * width) / 2.f);
	vertexs.push_back(position + (right * width) / 2.f);
	vertexs.push_back(position - (right * width) / 2.f);
}
