#pragma once
#include "TextEditor.h"
#include "stdafx.h"
#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "AudioEngine.h"
#include "Collision.h"
#include "Inputs.h"
#include "EntityContainer.h"
#include "Transform.h"
#include "LevelCreatorScene.h"

#include "Entity.h"
#include "TextEditor.h"

#include <map>

auto TileNumToString(unsigned int tileNum) -> std::string;

struct EntityProperties
{
	int translation[2] = { 0 };
	float rotation;
	bool isPicked = false;
	bool isEditable = false;
	bool isTileAttatch = false;
};

class EntityManager
{
public:

	auto EditEntity(Vector2 mousePos) -> void;
	auto ShowEntityInfo() -> void;
	auto ApplyProperties() -> int;
	auto InitializeProperties(std::string file_path) -> bool;
	auto EditText() -> void;
	auto EntityPicker() -> void;
	auto IsEntityPicked(const std::string& key) -> bool const;

	TextEditor editor;
	std::unordered_map<std::string, EntityProperties> properties;
	std::string fileToEdit = "";
	std::ifstream file;
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::CPlusPlus();
	Renderer* pRenderer;
	Vector2 mousePos_;
	Inputs* pInput;

private:
	auto SetMousePos(Vector2 mousePos) -> void;

};