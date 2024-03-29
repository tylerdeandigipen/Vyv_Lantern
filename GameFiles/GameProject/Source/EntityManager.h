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
#include "BehaviorDoor.h"

#include "Entity.h"
#include "TextEditor.h"

#include <map>

#include <windows.h>
#include <commdlg.h>
#include <sstream>
#include <fstream>

auto TileNumToString(unsigned int tileNum) -> std::string;

struct EntityProperties
{
	int translation[2] = { 0 };
	float rotation;
	bool isPicked = false;
	bool isEditable = false;
	bool isTileAttatch = false;
	Vector2 destination;
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
	auto fileReader(std::string filePath) -> void;

	TextEditor editor;
	std::unordered_map<std::string, EntityProperties> properties;
	std::string fileToEdit;
	
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::CPlusPlus();
	Renderer* pRenderer;
	Vector2 mousePos_;
	Inputs* pInput;

private:
	auto SetMousePos(Vector2 mousePos) -> void;



};