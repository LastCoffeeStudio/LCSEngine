#include "Globals.h"
#include "GameObject.h"
#include "Imgui/imgui.h"
#include "Component.h"

GameObject::GameObject() {}

GameObject::~GameObject() {}

void GameObject::addComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::deleteComponent(Component* component)
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if (*it == component)
		{
			components.erase(it);
			RELEASE(*it);
			return;
		}
	}
}

void GameObject::addGameObject(GameObject* gameObject)
{
	childs.push_back(gameObject);
}

void GameObject::deleteGameObject(GameObject* gameObject)
{
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if (*it == gameObject)
		{
			childs.erase(it);
			RELEASE(gameObject);
			return;
		}
	}
}

void GameObject::drawGui()
{
	ImGui::Checkbox("",&enable); ImGui::SameLine();
	ImGui::InputText("Name: ", name, IM_ARRAYSIZE(name));
	/*for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->drawGui();
	}*/
}

void GameObject::draw()
{
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->draw();
	}

	/*Mesh* mesh = nullptr;
	Material* material = nullptr;
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		switch ((*it)->type)
		{
			case MESH:
				mesh = (*it);
				break;
			case MATERIAL:
				material = (*it);
				break;
		}
	}

	//If it has a mesh and a material, draw it
	if (mesh != nullptr)
	{
		//Draw mesh with the material shaders and color depending of the input data
		//If it doesn't has material, draw it with default material and shaders
		if (material != nullptr)
		{
			
		}
		else
		{
			
		}
	}*/
}