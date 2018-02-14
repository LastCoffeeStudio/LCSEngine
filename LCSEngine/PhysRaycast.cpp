#include "Application.h"
#include "PhysRaycast.h"
#include "GameObject.h"
#include "Component.h"
#include "MeshComponent.h"
#include "ModuleSceneMain.h"
#include "MathGeoLib/src/Geometry/Triangle.h"
#include <queue>

PhysRaycast::PhysRaycast() {}

PhysRaycast::~PhysRaycast() {}

bool PhysRaycast::castRay(float3 origin, float3 direction, float distance)
{
	bool hits = false;
	float minDist = distance;
	queue<GameObject*> nodes;
	GameObject* root = App->sceneMain->root;

	for (vector<GameObject*>::iterator it = root->children.begin(); it != root->children.end(); ++it)
	{
		nodes.push(*it);
	}

	while (!nodes.empty())
	{
		GameObject* node = nodes.front();
		nodes.pop();

		for (vector<Component*>::iterator it = node->components.begin(); it != node->components.end(); ++it)
		{
			if ((*it)->typeComponent == MESH)
			{
				LineSegment l = LineSegment(origin, origin + (direction * distance));
				if (l.Intersects(node->obb.MinimalEnclosingAABB()))
				{
					MeshComponent* mesh = (MeshComponent*)(*it);
					Triangle t;
					l.Transform(node->id.Transposed().Inverted());

					for (unsigned int i = 0; i < mesh->verticesVBO.size(); i += 3)
					{
						t.a = mesh->verticesVBO[i];
						t.b = mesh->verticesVBO[i + 1];
						t.c = mesh->verticesVBO[i + 2];

						float dist;
						float3 intersectionPoint;
						if (l.Intersects(t, &dist, &intersectionPoint) && dist < minDist)
						{
							minDist = dist;
							gameObject = node;
							hits = true;
						}
					}
				}
			}
		}

		for (vector<GameObject*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
		{
			nodes.push(*it);
		}
	}

	return hits;
}

bool PhysRaycast::castRay(const LineSegment& l, float distance)
{
	return castRay(l.a, l.b - l.a, distance);
}