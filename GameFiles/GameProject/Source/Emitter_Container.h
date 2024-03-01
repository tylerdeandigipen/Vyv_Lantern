#pragma once

#include "Emitter.h"

class EmitterContainer
{
public:
	EmitterContainer();
	~EmitterContainer();
	void Add(Emitter* emitter);
	void Delete(std::string name);
	Emitter* FindByName(std::string name);
	bool CheckIfSource(Vector2 positon);

	void DeleteFlaggedEntities();

	Entity* Get(int position);
private:
	std::vector<Emitter*> Container;
	//Texture* texture;
};