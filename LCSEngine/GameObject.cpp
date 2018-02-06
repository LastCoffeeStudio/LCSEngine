#include "Globals.h"
#include "GameObject.h"
#include "Imgui/imgui.h"
#include "SDL\include\SDL_assert.h"
#include "Component.h"
#include "Glew/include/glew.h"
#include <stack>
#include "Application.h"
#include "ModuleSceneMain.h"
#include "Shader.h"
#include "ModuleCamera.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

GameObject::GameObject() {}

GameObject::GameObject(GameObject* parent, string name) : parent(parent), initialName(name) {
	GameObject::name = getFinalName(name);
	boundingBox.SetNegativeInfinity();
}

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

	/*DEBUG*/
	if (parent != nullptr)
	{
		char aux2[64];
		strcpy_s(aux2, 64, parent->name.c_str());
		ImGui::Text(aux2);
	}
	/*END DEBUG*/
	
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->drawGUI();
	}
}

void GameObject::draw()
{
	id = (parent != nullptr) ? parent->id : float4x4::identity;

	GLint idVertVBO = -1;
	unsigned int sizeVertVBO = 0;

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->isEnable)
		{
			switch ((*it)->typeComponent)
			{
				case TRANSFORM:
					id = ((TransformComponent*)(*it))->transform.Transposed()*id;
					break;
				case MESH:
					idVertVBO = ((MeshComponent*)(*it))->idVertVBO;
					sizeVertVBO = ((MeshComponent*)(*it))->verticesVBO.size();
					break;
			}
		}
	}

	//If has array of vertices, draw
	if (idVertVBO != -1 /*&& check if is inside the frustum*/)
	{
		/*Then change bool forDraw to true*/
		/*We need to modify this later to add the information in the queue for drawing*/
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		GLint modelLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "model_matrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &id[0][0]);

		GLint viewLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

		GLint projectLoc = glGetUniformLocation(App->sceneMain->shader->shaderProgram, "projection");
		glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

		glBindBuffer(GL_ARRAY_BUFFER, idVertVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, sizeVertVBO);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//drawBoundingBox();
}

string GameObject::getFinalName(string name)
{
	bool rep = false;

	for (vector<GameObject*>::iterator it = parent->children.begin(); it != parent->children.end(); ++it)
	{
		if ((*it)->name == name)
		{
			rep = true;
		}
	}

	if (!rep)
	{
		return name;
	}
	else {
		++nameNumber;
		string number = " (" + std::to_string(nameNumber) + ")";
		return getFinalName(initialName + number);
	}
}

/*void GameObject::drawBoundingBox()
{

}*/