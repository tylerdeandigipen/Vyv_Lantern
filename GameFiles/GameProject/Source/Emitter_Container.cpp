#include "Emitter_Container.h"
#include <stdlib.h>



EmitterContainer::EmitterContainer()
{
}

EmitterContainer::~EmitterContainer()
{
}

void EmitterContainer::Add(Emitter* emitter)
{
	Container.push_back(emitter);
}

Emitter* EmitterContainer::FindByName(std::string name)
{
	for (auto& emitter : Container)
	{
		if (emitter->GetName() == name)
		{
			return emitter;
		}
	}
	return NULL;
}

void EmitterContainer::Delete(std::string name)
{
	// Use an iterator to find the entity with the specified name
	auto it = std::remove_if(Container.begin(), Container.end(), [name](Emitter* emitter) {return emitter->GetName() == name; });

	// Delete the entities marked for removal
	for (auto removedEntity = it; removedEntity != Container.end(); ++removedEntity)
	{
		delete* removedEntity;
	}

	// Erase the removed entities from the vector
	Container.erase(it, Container.end());
}



bool EmitterContainer::CheckIfSource(Vector2 location)
{
	for (auto& emitter : Container)
	{
		if (emitter->GetPosition() == location)
		{
			return true;
		}
	}
	return false;
}