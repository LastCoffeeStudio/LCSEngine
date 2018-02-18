#include "Application.h"
#include "Globals.h"
#include "Utils.h"
#include "GameObject.h"
#include "Component.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "ComponentFactory.h"
#include "ModuleSceneMain.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleGUI.h"
#include "Shader.h"
#include "QuadTree.h"
#include "Imgui/imgui.h"
#include "SDL/include/SDL_assert.h"
#include "Glew/include/glew.h"
#include <queue>
#include "Model.h"

GameObject::GameObject() {}

GameObject::GameObject(GameObject* parent, string name) : parent(parent), initialName(name)
{
	GameObject::name = getFinalName(name);
	addComponent(new TransformComponent(this));
	aabb.SetNegativeInfinity();
	obb.SetNegativeInfinity();
	program = App->sceneMain->shader->programs[App->sceneMain->shader->defaultShaders[DEFAULTSHADER]];
}

GameObject::~GameObject() {}

void GameObject::preUpdate()
{
	id = (parent->parent != nullptr) ? parent->id : float4x4::identity;
	idVertVBO = -1;
	sizeVertVBO = 0;
	aabb.SetNegativeInfinity();
	obb.SetNegativeInfinity();

	if (staticPreviousValue != staticFlag && children.size() > 0)
	{
		App->gui->show_static_popup = true;
		staticPreviousValue = staticFlag;
	}

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->isEnable)
		{
			switch ((*it)->typeComponent)
			{
			case TRANSFORM:
				//Check if the element is static, in that case the transform is the identity
				if (staticFlag)
				{
					id = ((TransformComponent*)(*it))->transform.Transposed()*id;
				}
				else
				{
					id = ((TransformComponent*)(*it))->transform.Transposed()*id;
				}
				break;
			case MESH:
				idVertVBO = ((MeshComponent*)(*it))->idVertVBO;
				sizeVertVBO = ((MeshComponent*)(*it))->verticesVBO.size() * 3;	//vertices contains float3, that's why we multiply by 3
				idNormalVBO = ((MeshComponent*)(*it))->idNormVBO;
				sizeNormalVBO = ((MeshComponent*)(*it))->normalsVBO.size() * 3;
				idIdxVAO = ((MeshComponent*)(*it))->idIdxVAO;
				sizeIdxVAO = ((MeshComponent*)(*it))->indicesVAO.size();
				break;
			case MATERIAL:
				program = ((MaterialComponent*)(*it))->program;
			}
		}
	}
	
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		switch ((*it)->typeComponent)
		{
			case MESH:
				aabb.Enclose(&((MeshComponent*)(*it))->verticesVBO[0], ((MeshComponent*)(*it))->verticesVBO.size());
				obb = aabb.ToOBB();
				obb.Transform(id.Transposed());
				break;
			case CAMERA:
				((CameraComponent*)(*it))->frustum.pos = id.Transposed().TranslatePart();
				((CameraComponent*)(*it))->frustum.up = id.Transposed().WorldY();
				((CameraComponent*)(*it))->frustum.front = id.Transposed().WorldZ();
				break;
		}
	}
	
}

void GameObject::postUpdate() {}

void GameObject::addComponent(Component* component)
{
	bool alreadyHave = false;
	if (component->isUnique)
	{
		for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			if (component->typeComponent == (*it)->typeComponent)
			{
				alreadyHave = true;
			}
		}
	}
	if (!alreadyHave)
	{
		components.push_back(component);
		if (component->typeComponent == MESH && staticFlag)
		{
			App->sceneMain->rebuildQuadTree = true;
		}
	}
}

void GameObject::deleteComponent(Component* component)
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end();)
	{
		if (*it == component)
		{
			switch ((*it)->typeComponent)
			{
			case MESH:
				if (staticFlag)
				{
					App->sceneMain->rebuildQuadTree = true;
				}
				break;

			case MATERIAL:
				program = App->sceneMain->shader->programs[App->sceneMain->shader->defaultShaders[DEFAULTSHADER]];
				break;
			}

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

void GameObject::setStaticValueToChildrens()
{
	queue<GameObject*> queue;

	for (vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		queue.push((*it));
	}

	while (!queue.empty())
	{
		GameObject* gameObject = queue.front();
		queue.pop();

		gameObject->setStaticFlag(staticFlag);

		for (vector<GameObject*>::iterator it = gameObject->children.begin(); it != gameObject->children.end(); ++it)
		{
			queue.push((*it));
		}
	}
}

void GameObject::setStaticFlag(bool flag) {
	staticFlag = flag;
	staticPreviousValue = flag;
}

void GameObject::drawComponentsGui()
{
	ImGui::PushID("ActiveGameObject");
	ImGui::Checkbox("", &enable); ImGui::SameLine(0);
	ImGui::PopID();

	char aux[64];
	strcpy_s(aux, 64, name.c_str());

	ImGui::PushID("NameGameObject");
	if (ImGui::InputText("", aux, 64))
	{
		name = aux;
	}
	ImGui::PopID();

	ImGui::Separator();

	if(ImGui::Checkbox("Static", &staticFlag)) {
		App->sceneMain->rebuildQuadTree = true;
	};

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->drawGUI();
		ImGui::Separator();
	}

	ImGui::PushStyleColor(ImGuiCol_Button, { 0.25f, 0.27f, 0.3f, 1.f });
	if (ImGui::Button("Add Component", { 150, 20 }))
	{
		ImGui::OpenPopup("Add Component Popup");
	}
	ImGui::PopStyleColor(1);
	if (ImGui::BeginPopup("Add Component Popup"))
	{
		static ComponentFactory* factory = ComponentFactory::getInstance();

		if (ImGui::MenuItem("Mesh"))
		{
			addComponent(factory->getComponent(MESH, this));
		}
		else if (ImGui::MenuItem("Material"))
		{
			addComponent(factory->getComponent(MATERIAL, this));
		}
		else if (ImGui::MenuItem("Camera"))
		{
			addComponent(factory->getComponent(CAMERA, this));
		}

		ImGui::EndPopup();
	}
}

void GameObject::draw()
{
	if (idVertVBO != -1 && visible)
	{
		//Then change bool forDraw to true
		//We need to modify this later to add the information in the queue for drawing
		
		renderData data;
		data.id = id;
		data.idVertVBO = idVertVBO;
		data.sizeVertVBO = sizeVertVBO;
		data.idIdxVAO = idIdxVAO;
		data.idNormalVBO = idNormalVBO;
		data.sizeIdxVAO = sizeIdxVAO;
		data.sizeNormalVBO = sizeNormalVBO;
		App->renderer->renderQueue.insert(std::pair<GLuint,renderData>(program,data));

		//drawAABB();
		//drawOBB();
		
	}

	//TODO: AABB, OBB and Frustum data must be passed as VBO data, so we can add them to the render queue
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->typeComponent == CAMERA && (*it)->isEnable)
		{
			drawFrustum(((CameraComponent*)(*it))->frustum);
		}
	}
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
	else
	{
		++nameNumber;
		string number = " (" + std::to_string(nameNumber) + ")";
		return getFinalName(initialName + number);
	}
}

void GameObject::drawAABB()
{
	GLint modelLoc = glGetUniformLocation(program, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &id[0][0]);

	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

	GLint projectLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MinZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MinY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MinX(), aabb.MaxY(), aabb.MaxZ());
	glVertex3f(aabb.MaxX(), aabb.MaxY(), aabb.MaxZ());
	glEnd();
}

void GameObject::drawOBB()
{
	AABB aabbAux = obb.MinimalEnclosingAABB();
	float4x4 identity = float4x4::identity;

	GLint modelLoc = glGetUniformLocation(program, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

	GLint projectLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);

	glVertex3f(aabbAux.MinX(), aabbAux.MinY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MinY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MinY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MaxY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MinY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MinY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MinY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MaxY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MinY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MinY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MaxY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MaxY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MaxY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MaxY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MinY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MinY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MinY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MaxY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MaxY(), aabbAux.MinZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MaxY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MinY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MaxY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MinX(), aabbAux.MaxY(), aabbAux.MaxZ());
	glVertex3f(aabbAux.MaxX(), aabbAux.MaxY(), aabbAux.MaxZ());

	glEnd();
}

void GameObject::drawFrustum(Frustum frustum)
{
	float4x4 identity = float4x4::identity;

	GLint modelLoc = glGetUniformLocation(program, "model_matrix");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &identity[0][0]);

	GLint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, App->camera->getViewMatrix());

	GLint projectLoc = glGetUniformLocation(program, "projection");
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, App->camera->getProjectMatrix());

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);

	//Left Near
	glVertex3f(frustum.CornerPoint(0).x, frustum.CornerPoint(0).y, frustum.CornerPoint(0).z);
	glVertex3f(frustum.CornerPoint(2).x, frustum.CornerPoint(2).y, frustum.CornerPoint(2).z);

	//Top Near
	glVertex3f(frustum.CornerPoint(2).x, frustum.CornerPoint(2).y, frustum.CornerPoint(2).z);
	glVertex3f(frustum.CornerPoint(6).x, frustum.CornerPoint(6).y, frustum.CornerPoint(6).z);

	//Right Near
	glVertex3f(frustum.CornerPoint(6).x, frustum.CornerPoint(6).y, frustum.CornerPoint(6).z);
	glVertex3f(frustum.CornerPoint(4).x, frustum.CornerPoint(4).y, frustum.CornerPoint(4).z);

	//Bottom Near
	glVertex3f(frustum.CornerPoint(4).x, frustum.CornerPoint(4).y, frustum.CornerPoint(4).z);
	glVertex3f(frustum.CornerPoint(0).x, frustum.CornerPoint(0).y, frustum.CornerPoint(0).z);


	//Left Far
	glVertex3f(frustum.CornerPoint(1).x, frustum.CornerPoint(1).y, frustum.CornerPoint(1).z);
	glVertex3f(frustum.CornerPoint(3).x, frustum.CornerPoint(3).y, frustum.CornerPoint(3).z);

	//Top Far
	glVertex3f(frustum.CornerPoint(3).x, frustum.CornerPoint(3).y, frustum.CornerPoint(3).z);
	glVertex3f(frustum.CornerPoint(7).x, frustum.CornerPoint(7).y, frustum.CornerPoint(7).z);

	//Right Far
	glVertex3f(frustum.CornerPoint(7).x, frustum.CornerPoint(7).y, frustum.CornerPoint(7).z);
	glVertex3f(frustum.CornerPoint(5).x, frustum.CornerPoint(5).y, frustum.CornerPoint(5).z);

	//Bottom Far
	glVertex3f(frustum.CornerPoint(5).x, frustum.CornerPoint(5).y, frustum.CornerPoint(5).z);
	glVertex3f(frustum.CornerPoint(1).x, frustum.CornerPoint(1).y, frustum.CornerPoint(1).z);


	//Left-Top Cam->Far
	glVertex3f(frustum.pos.x, frustum.pos.y, frustum.pos.z);
	glVertex3f(frustum.CornerPoint(3).x, frustum.CornerPoint(3).y, frustum.CornerPoint(3).z);

	//Right-Top Cam->Far
	glVertex3f(frustum.pos.x, frustum.pos.y, frustum.pos.z);
	glVertex3f(frustum.CornerPoint(7).x, frustum.CornerPoint(7).y, frustum.CornerPoint(7).z);

	//Right-Bottom Cam->Far
	glVertex3f(frustum.pos.x, frustum.pos.y, frustum.pos.z);
	glVertex3f(frustum.CornerPoint(5).x, frustum.CornerPoint(5).y, frustum.CornerPoint(5).z);

	//Left-Bottom Cam->Far
	glVertex3f(frustum.pos.x, frustum.pos.y, frustum.pos.z);
	glVertex3f(frustum.CornerPoint(1).x, frustum.CornerPoint(1).y, frustum.CornerPoint(1).z);

	glEnd();
}

vector<Component*> GameObject::getComponents(TypeComponent type)
{
	vector<Component*> comps;

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->typeComponent == type)
		{
			comps.push_back((*it));
		}
	}

	return comps;
}