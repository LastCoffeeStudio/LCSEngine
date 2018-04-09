#include "Application.h"
#include "Globals.h"
#include "Utils.h"
#include "GameObject.h"
#include "Component.h"
#include "MeshComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "AudioListenerComponent.h"
#include "AudioSourceComponent.h"
#include "BillboardGridComponent.h"
#include "ScriptComponent.h"
#include "ComponentFactory.h"
#include "ModuleSceneMain.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAnimation.h"
#include "ModuleAudio.h"
#include "ModuleGUI.h"
#include "ModuleGameUI.h"
#include "ElementFactory.h"
#include "ElementGameUI.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UILabel.h"
#include "UIEditText.h"
#include "ModuleWindow.h"
#include "Shader.h"
#include "QuadTree.h"
#include "Model.h"
#include "AssetTexture.h"
#include "Imgui/imgui.h"
#include "SDL/include/SDL_assert.h"
#include "Glew/include/glew.h"
#include "DevIL/include/IL/il.h"
#include "MathGeoLib/src/Math/float4.h"
#include <queue>
#include <json.hpp>
#include "SaveLoadManager.h"

#include "ParticleSystemComponent.h"
#include "LightComponent.h"

GameObject::GameObject() {}

GameObject::GameObject(GameObject* parent, string name, unsigned int UUID) : parent(parent), initialName(name), UUID(UUID)
{
    if(parent != nullptr)
    {
        UUIDparent = parent->UUID;
    }
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

	updateComponents();
	updateElements();
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
		switch (component->typeComponent)
		{
			case MESH:
				if (staticFlag)
				{
					App->sceneMain->rebuildQuadTree = true;
				}
				break;
			case MATERIAL:
				//hasMaterial = true;
				break;
			default:
				break;
		}
	}
}

void GameObject::addElement(ElementGameUI* element)
{
	elements.push_back(element);
}

vector<Component*>::iterator GameObject::deleteComponent(Component* component)
{
	vector<Component*>::iterator it = components.begin();
	for (it; it != components.end();)
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
				{
					program = App->sceneMain->shader->programs[App->sceneMain->shader->defaultShaders[DEFAULTSHADER]];
					//hasMaterial = false;
					hasTexture = false;
					map<std::string, AssetTexture*>::iterator itMap = App->textures->textures.find(texPath);
					if (itMap != App->textures->textures.end())
					{
						(*itMap).second->numberUsages--;
						if ((*itMap).second->numberUsages <= 0)
						{
							App->textures->textures.erase(itMap);
						}
					}
				}
					break;
				case AUDIOLISTENER:
					AudioListenerComponent* audioListener = (AudioListenerComponent*)(*it);
					App->audio->unsetListener(audioListener->idAudioGameObj);
					break;
			}

			RELEASE(*it);
			it = components.erase(it);
			return it;
		}
		else
		{
			++it;
		}
	}
	return it;
}

vector<ElementGameUI*>::iterator GameObject::deleteElement(ElementGameUI* element) 
{
	vector<ElementGameUI*>::iterator it = elements.begin();
	for (it; it != elements.end();)
	{
		if (*it == element)
		{
			switch ((*it)->type)
			{
				case BUTTON:
					UIButton* element = (UIButton*)(*it);
					element->deleteElementButton();
					break;
			}

			RELEASE(*it);
			it = elements.erase(it);
			return it;
		}
		else
		{
			++it;
		}
	}
	return it;
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

void GameObject::save(nlohmann::json& conf) {
	conf["name"] = name;
	conf["UUID"] = UUID;
	conf["enable"] = enable;
	conf["staticFlag"] = staticFlag;
	conf["UUIDparent"] = UUIDparent;
	conf["nameNumber"] = nameNumber;
	conf["visible"] = visible;
	conf["enable"] = enable;
	nlohmann::json customJsont;
	SaveLoadManager::convertFloat4x4ToMyJSON(id, customJsont);
	conf["id"] = customJsont;
	SaveLoadManager::convertFloat4x4ToMyJSON(idBone, customJsont);
	conf["idBone"] = customJsont;
	SaveLoadManager::convertFloat3ToMyJSON(aabb.maxPoint, customJsont);
	conf["aabbMax"] = customJsont;
	SaveLoadManager::convertFloat3ToMyJSON(aabb.minPoint, customJsont);
	conf["aabbMin"] = customJsont;

	//Save Components
	list<nlohmann::json> componentsJson;
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		nlohmann::json compJson;
		(*it)->save(compJson);
		componentsJson.push_back(compJson);
	}
	conf["components"] = componentsJson;

	//Save elementes
	list<nlohmann::json> elementsJson;
	for (vector<ElementGameUI*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		nlohmann::json compJson;
		(*it)->save(compJson);
		elementsJson.push_back(compJson);
	}
	conf["elements"] = elementsJson;
}

void GameObject::load(nlohmann::json& conf) {
	name = conf.at("name").get<std::string>();
	UUID = conf.at("UUID").get<unsigned int>();
	enable = conf.at("enable").get<bool>();
	staticFlag = conf.at("staticFlag").get<bool>();
	UUIDparent = conf.at("UUIDparent").get<unsigned int>();
	nameNumber = conf.at("nameNumber").get<int>();
	visible = conf.at("visible").get<bool>();
	enable = conf.at("enable").get<bool>();
	SaveLoadManager::convertMyJSONtoFloat4x4(conf["id"], id);
	SaveLoadManager::convertMyJSONtoFloat4x4(conf["idBone"], idBone);
	SaveLoadManager::convertMyJSONtoFloat3(conf["aabbMax"], aabb.maxPoint);
	SaveLoadManager::convertMyJSONtoFloat3(conf["aabbMin"], aabb.minPoint);

	list<nlohmann::json> componentsJson = conf["components"];
	for (list<nlohmann::json>::iterator it = componentsJson.begin(); it != componentsJson.end(); ++it)
	{
		switch ((*it).at("typeComponent").get<int>())
		{
			case TRANSFORM:
			{
				TransformComponent* transformComp = new TransformComponent(this);
				transformComp->typeComponent = TRANSFORM;
				transformComp->load(*it);
				addComponent(transformComp);
			}
			break;
			case MATERIAL:
			{
				MaterialComponent* materialComp = new MaterialComponent(this);
				materialComp->typeComponent = MATERIAL;
				materialComp->load(*it);
				texPath = "";
				addComponent(materialComp);
			}
				break;
			case MESH:
			{
				MeshComponent* meshComp = new MeshComponent(this);
				meshComp->typeComponent = MESH;
				meshComp->load(*it);
				addComponent(meshComp);
			}
				break;
			case ANIMATION:
			{
				AnimationComponent* animationComp = new AnimationComponent(this);
				animationComp->typeComponent = ANIMATION;
				animationComp->load(*it);
				//addComponent(animationComp);
			}
				break;
			case AUDIOLISTENER:
			{
				AudioListenerComponent* audioListenerComp = new AudioListenerComponent(this);
				audioListenerComp->typeComponent = AUDIOLISTENER;
				audioListenerComp->load(*it);
				addComponent(audioListenerComp);
			}
				break;
			case AUDIOSOURCE:
			{
				AudioSourceComponent* audioSourceComp = new AudioSourceComponent(this);
				audioSourceComp->typeComponent = AUDIOSOURCE;
				audioSourceComp->load(*it);
				addComponent(audioSourceComp);
			}
				break;
			case BILLBOARDGRID:
			{
				BillboardGridComponent* billboardGridComponent = new BillboardGridComponent(this);
				billboardGridComponent->typeComponent = BILLBOARDGRID;
				billboardGridComponent->load(*it);
				addComponent(billboardGridComponent);
			}
				break;
			case LIGHT:
			{
				LightComponent* lightComponent = new LightComponent(this);
				lightComponent->typeComponent = LIGHT;
				lightComponent->load(*it);
				addComponent(lightComponent);
			}
				break;
		}
	}

	list<nlohmann::json> elementsJson = conf["elements"];
	for (list<nlohmann::json>::iterator it = elementsJson.begin(); it != elementsJson.end(); ++it)
	{
		switch ((*it).at("TypeElemeneGametUI").get<int>())
		{
			case BUTTON:
			{
				UIButton* butonElem = new UIButton(this);
				butonElem->type = BUTTON;
				butonElem->load(*it);
				addElement(butonElem);
			}
			break;
			case LABEL:
			{
				UILabel* labelElem = new UILabel(this);
				labelElem->type = LABEL;
				labelElem->load(*it);
				addElement(labelElem);
			}
			break;
			case IMAGE:
			{
				UIImage * imageElem = new UIImage(this);
				imageElem->type = IMAGE;
				imageElem->load(*it);
				addElement(imageElem);
			}
			break;
			case EDITTEXT:
			{
				UIEditText* editTextElem = new UIEditText(this);
				editTextElem->type = BUTTON;
				editTextElem->load(*it);
				addElement(editTextElem);
			}
			break;
		}
	}
}

void GameObject::drawComponentsElementsGui()
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

	for (vector<ElementGameUI*>::iterator it = elements.begin(); it != elements.end(); ++it)
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
		static ElementFactory* factoryElements = ElementFactory::getInstance();
		
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
        else if (ImGui::MenuItem("AudioListener"))
        {
            addComponent(factory->getComponent(AUDIOLISTENER, this));
        }
        else if (ImGui::MenuItem("AudioSource"))
        {
            addComponent(factory->getComponent(AUDIOSOURCE, this));
        }
		else if (ImGui::MenuItem("ParticleSystem"))
		{
			addComponent(factory->getComponent(PARTICLESYSTEM, this));
		}
		else if (ImGui::MenuItem("Billboard Grid"))
		{
			addComponent(factory->getComponent(BILLBOARDGRID, this));
		}
		else if (ImGui::MenuItem("Light"))
		{
			addComponent(factory->getComponent(LIGHT, this));
		}
		else if (ImGui::MenuItem("Script"))
		{
			addComponent(factory->getComponent(SCRIPT, this));
		}
		//Change all this and make same as components
		else if (ImGui::MenuItem("UI Image"))
		{
			addElement(factoryElements->getComponent(IMAGE, this, App->window->width/2 - BASIC_ELEMENT_GUI_SIZE/2, App->window->height / 2 - BASIC_ELEMENT_GUI_SIZE / 2, BASIC_ELEMENT_GUI_SIZE, BASIC_ELEMENT_GUI_SIZE, true));
		}
		else if (ImGui::MenuItem("UI Label"))
		{
			addElement(factoryElements->getComponent(LABEL, this, App->window->width / 2 - BASIC_ELEMENT_GUI_SIZE / 2, App->window->height / 2 - BASIC_ELEMENT_GUI_SIZE / 2, BASIC_ELEMENT_GUI_SIZE, BASIC_ELEMENT_GUI_SIZE, true));
		}
		else if (ImGui::MenuItem("UI Button"))
		{
			addElement(factoryElements->getComponent(BUTTON, this, App->window->width / 2 - BASIC_ELEMENT_GUI_SIZE / 2, App->window->height / 2 - BASIC_ELEMENT_GUI_SIZE / 2, BASIC_ELEMENT_GUI_SIZE, BASIC_ELEMENT_GUI_SIZE, true));
		}
		else if (ImGui::MenuItem("UI EditText"))
		{
			addElement(factoryElements->getComponent(EDITTEXT, this, App->window->width / 2 - BASIC_ELEMENT_GUI_SIZE / 2, App->window->height / 2 - BASIC_ELEMENT_GUI_SIZE / 2, BASIC_ELEMENT_GUI_SIZE, BASIC_ELEMENT_GUI_SIZE, true));
		}
		ImGui::EndPopup();
	}
}

void GameObject::draw()
{
	if (idVertVBO != -1 && visible)
	{
		/*for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			if ((*it)->typeComponent == MESH)
			{
				((MeshComponent*)(*it))->model->Draw();
			}
		}*/
		
		renderData data;
		data.id = id;
		data.idVertVBO = idVertVBO;
		data.sizeVertVBO = sizeVertVBO;
		data.idIdxVAO = idIdxVAO;
		data.idNormalVBO = idNormalVBO;
		data.sizeIdxVAO = sizeIdxVAO;
		data.sizeNormalVBO = sizeNormalVBO;
		data.textureID = texID;
		data.colorID = colorID;
		//data.hasMaterial = hasMaterial;
		data.hasTexture = hasTexture;
		data.textureCoordsID = texCoordsID;
		data.mode = GL_TRIANGLES;
		App->renderer->renderQueue.insert(std::pair<GLuint,renderData>(program,data));

		//drawAABB();
		//drawOBB();
		
	}

	//TODO: AABB, OBB and Frustum data must be passed as VBO data, so we can add them to the render queue
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		switch ((*it)->typeComponent)
		{
		case CAMERA:
			if ((*it)->isEnable)
			{
				drawFrustum(((CameraComponent*)(*it))->frustum);
			}
			break;
		case ANIMATION:
			((AnimationComponent*)(*it))->drawHierarchy();
			break;
		case BILLBOARDGRID:
		{
			renderData data;
			data.id = id;
			data.idVertVBO = ((BillboardGridComponent*)(*it))->idVertVBO;
			data.sizeVertVBO = ((BillboardGridComponent*)(*it))->verticesVBO.size();
			data.idIdxVAO = ((BillboardGridComponent*)(*it))->idIdxVAO;
			data.sizeIdxVAO = ((BillboardGridComponent*)(*it))->indicesVBO.size();
			data.textureID = ((BillboardGridComponent*)(*it))->texID;
			data.colorID = ((BillboardGridComponent*)(*it))->idColors;
			data.hasTexture = ((BillboardGridComponent*)(*it))->hasTexture;
			data.textureCoordsID = ((BillboardGridComponent*)(*it))->idTexCoords;
			data.mode = GL_TRIANGLES;
			App->renderer->renderQueue.insert(std::pair<GLuint,renderData>(program,data));
		}
		break;
		case PARTICLESYSTEM:
		{
			renderData data;
			data.id = id;
			data.idVertVBO = ((ParticleSystemComponent*)(*it))->idVertVBO;
			data.sizeVertVBO = ((ParticleSystemComponent*)(*it))->verticesVBO.size();
			data.idIdxVAO = ((ParticleSystemComponent*)(*it))->idIdxVAO;
			data.sizeIdxVAO = ((ParticleSystemComponent*)(*it))->indicesVBO.size();
			data.textureID = ((ParticleSystemComponent*)(*it))->texID;
			data.colorID = ((ParticleSystemComponent*)(*it))->idColors;
			data.hasTexture = ((ParticleSystemComponent*)(*it))->hasTexture;
			data.textureCoordsID = ((ParticleSystemComponent*)(*it))->idTexCoords;
			data.mode = GL_TRIANGLES;
			App->renderer->renderQueue.insert(std::pair<GLuint, renderData>(program, data));
		}
		break;
		default:
			break;
		}
	}
	for (vector<ElementGameUI*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		App->gameUI->elements.push_back((*it));
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

Component* GameObject::getComponent(TypeComponent type)
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->typeComponent == type)
		{
			return (*it);
		}
	}

	return nullptr;
}

void GameObject::instantiate()
{
	//addGameObject(new GameObject(App->sceneMain->currentObject, "GameObject", uuid()));
}

void GameObject::updateBones(const AnimationComponent* anim)
{
	GameObject* root = anim->rootBone;

	queue<GameObject*> gameObjects;
	gameObjects.push(root);

	while (!gameObjects.empty())
	{
		GameObject* node = gameObjects.front();
		gameObjects.pop();

		float3 position = { 0.f, 0.f, 0.f };
		Quat rotation = { 0.f, 0.f, 0.f, 0.f };

		if (App->animations->getTransform(anim->idAnim, node->name.c_str(), position, rotation))
		{
			TransformComponent* transform = (TransformComponent*)(node->getComponent(TRANSFORM));
			transform->setTranslate(position);
			transform->setRotate(rotation);
		}

		for (vector<GameObject*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
		{
			gameObjects.push(*it);
		}
	}
}

void GameObject::updateTransformBones(const AnimationComponent* anim)
{
	GameObject* rootBone = anim->rootBone;

	queue<GameObject*> gameObjects;
	
	for (vector<GameObject*>::iterator it = rootBone->children.begin(); it != rootBone->children.end(); ++it)
	{
		gameObjects.push(*it);
	}

	while (!gameObjects.empty())
	{
		GameObject* node = gameObjects.front();
		gameObjects.pop();

		node->idBone = node->parent->idBone*((TransformComponent*)node->getComponent(TRANSFORM))->transform;

		for (vector<GameObject*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
		{
			gameObjects.push(*it);
		}
	}
}

void GameObject::updateVertices(const AnimationComponent* anim)
{
	queue<GameObject*> gameObjects;
	
	for (vector<GameObject*>::iterator it = children.begin(); it != children.end(); ++it)
	{
		gameObjects.push(*it);
	}

	while (!gameObjects.empty())
	{
		GameObject* node = gameObjects.front();
		gameObjects.pop();

		MeshComponent* mesh = (MeshComponent*)node->getComponent(MESH);

		if (mesh != nullptr && mesh->bones.size() > 0)
		{
			std::fill(mesh->verticesVBO.begin(), mesh->verticesVBO.end(), float3(0.f,0.f,0.f));
			for (vector<Bone*>::iterator it = mesh->bones.begin(); it != mesh->bones.end(); ++it)
			{
				for (vector<Weight*>::iterator itW = (*it)->weights.begin(); itW != (*it)->weights.end(); ++itW)
				{
					float4x4 transform = anim->joints.find((*it)->name)->second->idBone;
					float4 finalPos = (*itW)->weight * (transform * (*it)->bind * float4(mesh->originalVertices[(*itW)->vertex], 1.f));
					mesh->verticesVBO[(*itW)->vertex] += float3(finalPos.x,finalPos.y,finalPos.z);
				}
			}

			mesh->updateVerticesBuffer();
			//At the end, update buffer of vertices on gpu
		}

		for (vector<GameObject*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
		{
			gameObjects.push(*it);
		}
	}
}

void GameObject::updateComponents()
{
	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		switch ((*it)->typeComponent)
		{
		case ANIMATION:
			if (((AnimationComponent*)(*it))->idAnim != 0)
			{
				updateBones((AnimationComponent*)(*it));
				updateTransformBones((AnimationComponent*)(*it));
				updateVertices((AnimationComponent*)(*it));
			}
			break;
		case SCRIPT:
			//((ScriptComponent*)(*it))->updateScripts();
		default:
			break;
		}
	}

	for (vector<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->isEnable)
		{
			switch ((*it)->typeComponent)
			{
			case TRANSFORM:
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
				texCoordsID = ((MeshComponent*)(*it))->idTexCoords;
				colorID = ((MeshComponent*)(*it))->idColors;

				break;
			case MATERIAL:
				program = ((MaterialComponent*)(*it))->program;
				if (((MaterialComponent*)(*it))->textureChanged && ((MaterialComponent*)(*it))->textureName != texPath)
				{
					updateTexture(texPath, ((MaterialComponent*)(*it))->textureName, texID, hasTexture);
					((MaterialComponent*)(*it))->textureChanged = false;
				}

				if (((MaterialComponent*)(*it))->colorChanged)
				{
					MeshComponent* mesh = (MeshComponent*)getComponent(MESH);
					if (mesh != nullptr)
					{
						mesh->updateColor(((MaterialComponent*)(*it))->color);
					}
					((MaterialComponent*)(*it))->colorChanged = false;
				}
				break;
			case AUDIOLISTENER:
				App->audio->updatePositionListener(((AudioListenerComponent*)(*it))->idAudioGameObj, id);
				break;
			case AUDIOSOURCE:
				App->audio->updatePositionAudioSource(((AudioSourceComponent*)(*it))->idAudioGameObj, id);
				break;
			case BILLBOARDGRID:
				((BillboardGridComponent*)(*it))->calculateVertexs();
				break;
			case PARTICLESYSTEM:
				((ParticleSystemComponent*)(*it))->updateParticles(App->deltaTime);
				break;
			default:
				break;
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
		default:
			break;
		}
	}
}

void GameObject::updateElements()
{
	for (vector<ElementGameUI*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		switch ((*it)->type)
		{
			case LABEL:
			{
				((UILabel*)(*it))->update();
				break;
			}
			case BUTTON:
			{
				((UIButton*)(*it))->update();
				UIImage* image = ((UIButton*)(*it))->activeImage;
				if (image->textureChanged)
				{
					updateTexture(image->texName, image->textureName, image->texID, image->hasTexture);
					image->textureChanged = false;
				}
				break;
			}
			case IMAGE:
				if (((UIImage*)(*it))->textureChanged)
				{
					updateTexture(((UIImage*)(*it))->texName, ((UIImage*)(*it))->textureName, ((UIImage*)(*it))->texID, ((UIImage*)(*it))->hasTexture);
					((UIImage*)(*it))->textureChanged = false;
				}
				break;
			case EDITTEXT:
			{
				((UIEditText*)(*it))->update();
				UIImage* background = ((UIEditText*)(*it))->background;
				UIImage* selected = ((UIEditText*)(*it))->selected;
				if (background->textureChanged)
				{
					updateTexture(background->texName, background->textureName, background->texID, background->hasTexture);
					background->textureChanged = false;
				}
				if (selected->textureChanged)
				{
					updateTexture(selected->texName, selected->textureName, selected->texID, selected->hasTexture);
					selected->textureChanged = false;
				}
				break;
			}
			default: 
				break;
		}
	}
}

void GameObject::updateTexture(string& lastPath, const char* newPath, GLuint& id, bool& hasText)
{
	map<std::string, AssetTexture*>::iterator itTexture = App->textures->textures.find(lastPath);
	if (itTexture != App->textures->textures.end())
	{
		(*itTexture).second->numberUsages--;
		if ((*itTexture).second->numberUsages <= 0)
		{
			App->textures->textures.erase(itTexture);
		}
	}

	lastPath = newPath;
	map<std::string, AssetTexture*>::iterator itNewTexture = App->textures->textures.find(lastPath);
	if (itNewTexture != App->textures->textures.end())
	{
		id = (*itNewTexture).second->ID;
		hasText = true;
		(*itNewTexture).second->numberUsages++;
	}
	else if (App->textures->loadTexture(lastPath.c_str()))
	{
		id = App->textures->textures[lastPath]->ID;
		hasText = true;
	}
	else
	{
		id = 0;
		hasText = false;
	}
}