//------------------------------------------------------------------------------
//
// File Name:	Entity.cpp
// Author(s):	Michael Howard
// Purpose:		Base for all game objects (could do object then this shall see)
//				(adapted from Schillings usage in cs230)
//
// Copyright  © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include <algorithm>
#include "Component.h"
#include "ComponentFactory.h"
#include "Behavior.h"
#include "BehaviorPlayer.h"
#include "Entity.h"
#include "Transform.h"
#include "ImageBuffer.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Used to sort components using their type Id.
struct ComponentSorter
{
	bool operator()(Component* left, Component* right)const
	{
		return left->type() < right->type();
	}
};

Entity::Entity() : isDestroyed(false), components(), name{}, image(NULL), mName(), isLight(false), isObject(false)
{

}

Entity::Entity(std::string type, const std::string file) : isDestroyed(0), components(), name{}, image(NULL), mName(), isLight(false), isObject(false)
{
	if (type.compare("Object") == 0)
	{
		CreateImage(file);
		isObject = true;
	}
	else if (type.compare("Image") == 0)
	{

		//CreateLight();
		isLight = true;
	}
	else
	{

	}

}


Entity::Entity(Entity const& ent) : isDestroyed(ent.isDestroyed), name{}, components(ent.components)
{
	if (ent.name)
		strcpy_s(name, _countof(name), ent.name);

	int i = 0;
	for (auto component : ent.components)
	{
		components[i] = component->Clone();
		components[i]->Parent(this);
		i++;
	}
}

Entity::~Entity()
{
	if (image)
		delete image;
//	if (light)
//		delete light;
}

Entity* Entity::Clone()
{
	if (this)
		return new Entity(*this);
	else
		return NULL;
}

void Entity::FreeComponents()
{
	for (auto component : components)
	{
		if (component != NULL)
		{
			delete component;
			component = NULL;
		}
	}
}

void Entity::Destroy()
{
	isDestroyed = true;
}

bool Entity::IsDestroyed() { return isDestroyed; }
bool Entity::IsLight() { return isLight; };
bool Entity::IsObject() { return isObject; };

void Entity::Read(json const& jsonData)
{
	ComponentFactory* factory = ComponentFactory::GetInstance();

	mName = jsonData["Name"];

	for (auto componentData : jsonData["Components"])
	{
		std::string type = componentData["Type"];
		if (Component* component = factory->CreateComponent(type))
		{
			component->Read(componentData);
			Add(component);
		}
	}
	if (isObject)
		Has(Transform)->SetTranslation(&(image->position));
}

void Entity::Add(Component* component)
{
	if (component)
	{
		// Set the component's parent.
		component->Parent(this);

		// Add the component to the components list.
		components.push_back(component);

		// Sort the list for faster access using a binary search.
		std::sort(components.begin(), components.end(), ComponentSorter());
	}
}

std::string Entity::ObjectName()
{
	return "Object";
}

//std::string Entity::ObjectName()
//{
//	return "Light";
//}

void Entity::SetName(const char* _name)
{
	if (_name)
	{
		strcpy_s(name, _countof(name), _name);
	}
}

bool Entity::IsNamed(const char* _name)
{
	if (_name && name)
	{
		if (!strcmp(name, _name))
			return true;
		else
			return false;
	}

	return false;
}

// could possilbe redo this for a different version and what works best
Component* Entity::BinarySearch(Component::TypeEnum type) const
{
	size_t begin = 0;
	size_t end = components.size();

	while (begin < end)
	{
		size_t mid = (begin + end) / 2;
		if (components[mid]->type() < type)
			begin = mid + 1;
		else
			end = mid;
	}
	if ((begin < components.size()) && (components[begin]->type() == type))
	{
		// Return the first component found that matches the specified type.
		return components[begin];
	}
	else
	{
		// No component matches the specified type.
		return NULL;
	}
}

Component* Entity::Get(Component::TypeEnum type) const
{
	return BinarySearch(type);
}

const char* Entity::GetName()
{
	if (name)
	{
		return name;
	}
	else
		return NULL;
}

const char* Entity::GetName() const
{
	if (name)
	{
		return name;
	}
	else
		return NULL;
}

void Entity::Update(float dt)
{
	for (auto component : components)
	{
		if (component)
		{
			component->Update(dt);
		}
	}
}

void Entity::Render(void)
{
	for (auto component : components)
	{
		component->Render();
	}
}

void Entity::CreateImage(const std::string _file)
{
	image = new ImageBuffer(_file);
	image->layer = 1;
}

void Entity::AddToRenderer(Renderer* pixel)
{
	pixel->AddObject(image);
}

void Entity::SetInputHandler(Inputs* input)
{
	if (input)
		Has(Behavior)->Type()->SetInputHandler(input);
}
