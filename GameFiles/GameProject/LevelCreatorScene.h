//------------------------------------------------------------------------------
//
// File Name:	LevelCreatorScene.h
// Author(s):	TayLee Young, Louis Wang, Michael Howard, Tyler Dean
// Purpose:		Test scene for our game
//
// Copyright  ?2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Engine.h"
#include "Scene.h"
#include "SceneSystem.h"
#include "AudioEngine.h"
#include "Collision.h"
#include "Inputs.h"
#include "EntityContainer.h"
#include "Transform.h"

#include "Entity.h"
#include "TextEditor.h"

#include <map>


class Scene;

class LevelCreatorScene : public Scene
{
public:
	
	LevelCreatorScene();

	Engine::EngineCode Load(void) override;
	Engine::EngineCode Init(void) override;
	Engine::EngineCode Exit(void) override;
	Engine::EngineCode Unload(void) override;
	void Update(float dt) override;
	void Render(void) override;
	bool winState = false;
private:
	void ImGuiInterg();
	void ImGuiWindow();
	void ToolHandler();
	Vector2 PlaceTile(Vector2 tilePos);
	Vector2 PlaceObject(Vector2 objectPos);
	void ToolPan(Inputs* inputHandler, Vector2 CursourP);
	void ToolBrush(Inputs* inputHandler, Vector2 CursourP);
	void ToolEyedroper(Inputs* inputHandler, Vector2 CursourP);
	void ToolCenter(Inputs* inputHandler);
	void ToolSquareFill(Inputs* inputHandler, Vector2 CursourP);
	int CreateCircleEntity();
	int CreateDoorEntity();
	int CreateMirrorEntity();


	std::string currentGameObjectsList = "./Data/GameObjects/ObjectListLevelBuilder.json";
	std::string currentTileMapList;
	char myTextBuffer[256]{};
	char LoadBuffer[256]{};

	enum ObjType
	{
		LIGHT_EMITTER,
		COUNT
	};
};

struct EntityProperties
{
	int translation[2] = { 0 };
	Vector2 rotation;
	// bool isCollidable;
	bool isPicked = false;
};

class EntityManager
{
public:
	[[nodiscard]] void EditEntity(Vector2 mousePos)
	{
		if (isEditable)
		{
			SetMousePos(mousePos);
			EntityPicker();
		}
	}

	[[noreturn]] void ShowEntityInfo()
	{
		for (int i = 0; i < EntityContainer::GetInstance()->CountEntities(); i++)
		{
			if (ImGui::TreeNode(("Entity %s", (*EntityContainer::GetInstance())[i]->GetRealName().c_str())))
			{
				//ImGui::Text("Name: %s", (*EntityContainer::GetInstance())[i]->GetName());
				ImGui::Text("Entity Number: %d", i);

				ImGui::Text("Transform: (%f, %f)", properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[0], properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[1]);
				ImGui::Text("Rotation: (%f, %f)", properties[(*EntityContainer::GetInstance())[i]->GetRealName()].rotation);
				ImGui::SliderInt2("Test Transform", properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation, -10.f, 100.f);

				ImGui::Checkbox("isEditable", &isEditable);
				ImGui::Text(isEditable ? "true" : "false");
				ImGui::Text((properties[(*EntityContainer::GetInstance())[i]->GetRealName()].isPicked ? "true" : "false"));

				if (ImGui::Button("Delete"))
				{
					EntityContainer::GetInstance()->RemoveEntity((*EntityContainer::GetInstance())[i]);
					break;
				}

				pRenderer->objects[i]->position.x = properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[0];
				pRenderer->objects[i]->position.y = properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[1];

				ImGui::TreePop();
			}
		}
	}

	[[nodiscard]] int ApplyProperties()
	{
		for (int i = 0; i < EntityContainer::GetInstance()->CountEntities(); i++)
		{
			std::string filename = (*EntityContainer::GetInstance())[i]->GetFilePath(); // Replace with your actual JSON file path

			// Read the JSON file
			std::ifstream input_file(filename);
			if (!input_file.is_open())
			{
				std::cerr << "Error opening file" << std::endl;
				return 1;
			}

			json j;
			input_file >> j;
			input_file.close();

			// Modify the translation values
			for (auto& component : j["Components"])
			{
				if (component["Type"] == "Transform")
				{
					// Set new values for x and y
					component["translation"]["x"] = properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[0]; // Replace with the new X value
					component["translation"]["y"] = properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[1]; // Replace with the new Y value
					break;
				}
			}

			// Write the modified JSON back to the file
			std::ofstream output_file(filename);
			if (!output_file.is_open())
			{
				std::cerr << "Error opening file for writing" << std::endl;
				return 1;
			}

			output_file << j.dump(4); // Writing with an indentation of 4 spaces
			output_file.close();
		}
		return EXIT_SUCCESS;
	}

	[[nodiscard]] bool InitializeProperties(std::string file_path)
	{
		//initialize level data
		EntityContainer::GetInstance()->ReadEntities(file_path);

		for (int i = 0; i < EntityContainer::GetInstance()->CountEntities(); i++)
		{
			Transform* t = (*EntityContainer::GetInstance())[i]->Has(Transform);
			properties[(*EntityContainer::GetInstance())[i]->GetRealName()] = EntityProperties{ {static_cast<int>(std::floorf(t->GetTranslation()->x)), static_cast<int>(std::floorf(t->GetTranslation()->y))}, {0.f, 0.f}, false };
		}

		bool isEditing = true;

		file = std::ifstream(fileToEdit);

		if (file.good())
		{
			std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			editor.SetText(str);
		}

		return true; //Initialization success!
	}

	[[noreturn]] void EditText()
	{
		auto cpos = editor.GetCursorPosition();

		editor.SetLanguageDefinition(lang);

		ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					auto textToSave = editor.GetText();
					/// save text....
				}
				if (ImGui::MenuItem("Load"))
				{
					ImGui::Begin("Select File", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);
					ImGui::End();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					editor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
					editor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
					editor.Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
					editor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
					editor.Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
					editor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					editor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					editor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					editor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
					editor.SetPalette(TextEditor::GetRetroBluePalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
			editor.IsOverwrite() ? "Ovr" : "Ins",
			editor.CanUndo() ? "*" : " ",
			editor.GetLanguageDefinition().mName.c_str(), fileToEdit);

		editor.Render("TextEditor");

		ImGui::End();
	}

	void EntityPicker()
	{
		if (isEditable)
		{
			for (int i = 0; i < EntityContainer::GetInstance()->CountEntities(); i++)
			{
				if ((properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[0] + 15) >= mousePos_.x && (properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[0]) <= mousePos_.x &&
					(properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[1] + 15) >= mousePos_.y && (properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[1]) <= mousePos_.y)
				{
					if (pInput->mouseButtonDown(SDL_BUTTON_LEFT))
					{
						properties[(*EntityContainer::GetInstance())[i]->GetRealName()].isPicked = true;
					}
					else
					{
						properties[(*EntityContainer::GetInstance())[i]->GetRealName()].isPicked = false;
					}
				}

				if (properties[(*EntityContainer::GetInstance())[i]->GetRealName()].isPicked == true)
				{
					properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[0] = mousePos_.x - 8;
					properties[(*EntityContainer::GetInstance())[i]->GetRealName()].translation[1] = mousePos_.y - 8;
				}
			}
		}
	}

	void SetMousePos(Vector2 mousePos)
	{
		mousePos_.x = mousePos.x / pRenderer->screenScale;
		mousePos_.y = mousePos.y / pRenderer->screenScale;
	}

	TextEditor editor;
	std::unordered_map<std::string, EntityProperties> properties;
	std::string fileToEdit = "Data/GameObjects/Circle.json";
	std::ifstream file;
	TextEditor::LanguageDefinition lang = TextEditor::LanguageDefinition::CPlusPlus();
	Renderer* pRenderer;
	bool isEditable = false;
	Vector2 mousePos_;
	Inputs* pInput;
};

Scene* LevelCreatorSceneGetInstance(void);
