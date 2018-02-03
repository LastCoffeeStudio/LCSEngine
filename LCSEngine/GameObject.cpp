#include "Globals.h"
#include "GameObject.h"
#include "Imgui/imgui.h"
#include "Component.h"
#include <stack>

GameObject::GameObject() {}

GameObject::GameObject(GameObject* parent, string name) : parent(parent), name(name) {}

GameObject::~GameObject() {}

void GameObject::addComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::deleteComponent(Component* component)
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end();)
	{
		if (*it == component)
		{
			RELEASE(*it);
			it = components.erase(it);
			return;
		}
		else
		{
			++it;
		}
	}
}

void GameObject::addGameObject(GameObject* gameObject)
{
	children.push_back(gameObject);
}

void GameObject::deleteGameObject()
{
	stack<GameObject*> gameObjects;
	gameObjects.push(this);

	for (vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		gameObjects.push(*it);
	}

	while (!gameObjects.empty())
	{
		GameObject* current = gameObjects.top();
		gameObjects.pop();
		GameObject* parent = current->parent;
		bool erased = false;
		//Delete info in parent children
		for (vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end() && !erased;)
		{
			if ((*it) == current)
			{
				it = parent->children.erase(it);
				erased = true;
			}
			else
			{
				++it;
			}
		}
		parent = nullptr;

		//Delete all components
		for (vector<Component*>::iterator it = current->components.begin(); it != current->components.end();)
		{
			//(*it)->cleanUp();
			RELEASE(*it);
			it = current->components.erase(it);
		}
		current->components.clear();

		//Push in the queue all children
		for (vector<GameObject*>::iterator it = current->children.begin(); it != current->children.end(); ++it)
		{
			gameObjects.push(*it);
		}
		current->children.clear();

		RELEASE(current);
	}
}

void GameObject::drawComponentsGui()
{
	ImGui::Checkbox("",&enable); ImGui::SameLine();
	char aux[64];
	strcpy_s(aux, 64, name.c_str());
	if (ImGui::InputText("Name: ", aux, 64))
	{
		name = aux;
	}
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->drawGUI();
	}
}

void GameObject::draw()
{
	for (vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
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