
#include "EntityManager.h"

auto EntityManager::ShowEntityInfo() -> void
{
	if (!pRenderer || !pInput)
		return;

	LevelCreatorScene* creator = reinterpret_cast<LevelCreatorScene*>(LevelCreatorSceneGetInstance());

	if (!creator)
		return;

	if (!creator->tempEntities.empty())
	{
		if (ImGui::TreeNode("*Entities:"))
		{
			for (int i = 0; i < creator->tempEntities.size(); i++)
			{
				if (!creator->tempEntities[i])
					continue;

				if (ImGui::TreeNode(("Entity %s", creator->tempEntities[i]->key.c_str())))
				{
					ImGui::Text("Entity Number: %d", i);
					ImGui::Text("Mouse Pos: %f %f", mousePos_.x, mousePos_.y);

					ImGui::Text("Translation: (%d, %d)", properties[creator->tempEntities[i]->key].translation[0], properties[creator->tempEntities[i]->key].translation[1]);
					ImGui::Text("Rotation: (%f, %f)", properties[creator->tempEntities[i]->key].rotation);
					ImGui::Text("Size: (%d, %d)", static_cast<int>(pRenderer->objects[i]->size.x), static_cast<int>(pRenderer->objects[i]->size.y));

					ImGui::SliderInt2("Modify Translation", properties[creator->tempEntities[i]->key].translation, -500, 500);
					creator->tempEntities[i]->Has(Transform)->SetTranslation({ static_cast<float>(properties[creator->tempEntities[i]->key].translation[0]), static_cast<float>(properties[creator->tempEntities[i]->key].translation[1]) });

					ImGui::Checkbox((properties[creator->tempEntities[i]->key].isEditable ? "Entity edit enabled" : "Entity edit disabled"), &properties[creator->tempEntities[i]->key].isEditable);
					ImGui::Checkbox((properties[creator->tempEntities[i]->key].isTileAttatch ? "Tile attatch enabled" : "Tile attatch disabled"), &properties[creator->tempEntities[i]->key].isTileAttatch);

					if (ImGui::Button("Delete"))
					{
						if (!creator->tempEntities.empty())
						{
							for (auto it = creator->tempEntities.begin(); it != creator->tempEntities.end(); ++it)
							{
								if (*it == creator->tempEntities[i])
								{
									if ((*it)->key.compare(creator->tempEntities[i]->key) == 0)
									{
										(*it)->FreeComponents();
										delete* it;
										*it = nullptr;
										break;
									}
								}
							}
						}
					}

					if (!creator->tempEntities[i])
					{
						continue;
					}
					pRenderer->objects[i]->position.x = properties[creator->tempEntities[i]->key].translation[0];
					pRenderer->objects[i]->position.y = properties[creator->tempEntities[i]->key].translation[1];

					ImGui::TreePop();
				}
			}
		}
	}
}

auto EntityManager::ApplyProperties() -> int
{
	if (!pRenderer)
		return EXIT_FAILURE;

	LevelCreatorScene* creator = reinterpret_cast<LevelCreatorScene*>(LevelCreatorSceneGetInstance());
	for (int i = 0; i < creator->tempEntities.size(); i++)
	{
		std::string filename = creator->tempEntities[i]->GetFilePath(); // Replace with your actual JSON file path

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
				component["translation"]["x"] = properties[creator->tempEntities[i]->key].translation[0]; // Replace with the new X value
				component["translation"]["y"] = properties[creator->tempEntities[i]->key].translation[1]; // Replace with the new Y value
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

auto EntityManager::EntityPicker() -> void
{
	LevelCreatorScene* creator = reinterpret_cast<LevelCreatorScene*>(LevelCreatorSceneGetInstance());
	for (int i = 0; i < creator->tempEntities.size(); i++)
	{
		if (creator->tempEntities[i] != nullptr)
		{
			if (properties[creator->tempEntities[i]->key].isEditable == true)
			{
				if ((properties[creator->tempEntities[i]->key].translation[0] + pRenderer->objects[i]->size.x) >= mousePos_.x && (properties[creator->tempEntities[i]->key].translation[0]) <= mousePos_.x &&
					(properties[creator->tempEntities[i]->key].translation[1] + pRenderer->objects[i]->size.y) >= mousePos_.y && (properties[creator->tempEntities[i]->key].translation[1]) <= mousePos_.y)
				{
					if (pInput->mouseButtonDown(SDL_BUTTON_LEFT))
					{
						properties[creator->tempEntities[i]->key].isPicked = true;
					}
					else
					{
						properties[creator->tempEntities[i]->key].isPicked = false;
					}
				}

				if (properties[creator->tempEntities[i]->key].isPicked == true)
				{
					if (properties[creator->tempEntities[i]->key].isTileAttatch == true)
					{
						properties[creator->tempEntities[i]->key].translation[0] = (static_cast<int>(mousePos_.x - pRenderer->objects[i]->size.x / 2) % 8 == 0) ? static_cast<int>(mousePos_.x - pRenderer->objects[i]->size.x / 2) + 4 : properties[creator->tempEntities[i]->key].translation[0];
						properties[creator->tempEntities[i]->key].translation[1] = (static_cast<int>(mousePos_.y - pRenderer->objects[i]->size.y / 2) % 8 == 0) ? static_cast<int>(mousePos_.y - pRenderer->objects[i]->size.y / 2) + 4 : properties[creator->tempEntities[i]->key].translation[1];
					}
					else
					{
						properties[creator->tempEntities[i]->key].translation[0] = static_cast<int>(mousePos_.x - pRenderer->objects[i]->size.x / 2);
						properties[creator->tempEntities[i]->key].translation[1] = static_cast<int>(mousePos_.y - pRenderer->objects[i]->size.y / 2);
					}
				}
			}
		}
	}
}

auto EntityManager::EditText() -> void
{
	if (!pRenderer || !pInput)
		return;

	auto cpos = editor.GetCursorPosition();

	editor.SetLanguageDefinition(lang);

	ImGui::Begin("Text Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(600, 800), ImGuiCond_FirstUseEver);

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

auto EntityManager::InitializeProperties(std::string file_path) -> bool
{
	if (!pRenderer)
		return false;

	LevelCreatorScene* creator = reinterpret_cast<LevelCreatorScene*>(LevelCreatorSceneGetInstance());
	EntityContainer::GetInstance()->ReadEntities(file_path);

	for (int i = 0; i < creator->tempEntities.size(); i++)
	{
		if (creator->tempEntities[i])
		{
			Transform* t = creator->tempEntities[i]->Has(Transform);
			properties[creator->tempEntities[i]->key] = EntityProperties{ {static_cast<int>(std::floorf(t->GetTranslation()->x)), static_cast<int>(std::floorf(t->GetTranslation()->y))}, {0.f}, false, false };
		}
	}

	file = std::ifstream(fileToEdit);

	if (file.good())
	{
		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		editor.SetText(str);
	}
	else
	{
		std::cerr << "Error opening file" << std::endl;
		return false;
	}

	return true;
}

auto EntityManager::SetMousePos(Vector2 mousePos) -> void
{
	mousePos_.x = (mousePos.x) / pRenderer->screenScale + pRenderer->GetCameraPosition().x;
	mousePos_.y = (mousePos.y) / pRenderer->screenScale + pRenderer->GetCameraPosition().y;
}

auto EntityManager::IsEntityPicked(const std::string& key) -> bool const
{
	auto it = properties.find(key);
	return it != properties.end() ? it->second.isPicked : false;
}

auto EntityManager::EditEntity(Vector2 mousePos) -> void
{
	if (!pRenderer || !pInput)
		return;

	SetMousePos(mousePos);
	EntityPicker();
}

auto TileNumToString(unsigned int tileNum) -> std::string
{
	static const std::map<unsigned int, std::string> tileNames =
	{
		{ 0, "Empty" },
		{ 1, "Black" },
		{ 2, "BottomLedge1" },
		{ 3, "BottomLedge2" },
		{ 4, "LeftLedge" },
		{ 5, "RightLedge" },
		{ 6, "WallTop" },
		{ 7, "WallTopLedge" },
		{ 8, "WallCracked1" },
		{ 9, "WallCracked2" },
		{ 10, "WallWhole" },
		{ 11, "WallTrim1" },
		{ 12, "WallLight" },
		{ 13, "WallTrim2" },
		{ 14, "WallTrim3" },
		{ 15, "SmallRock" },
		{ 16, "PurpleFloor" },
		{ 17, "PurpleFloorDeco1" },
		{ 18, "PurpleFloorDeco2" },
		{ 19, "PurpleFloorDeco3" },
		{ 20, "PurpleFloorDeco4" },
		{ 21, "PurpleFloorDeco5" },
		{ 22, "BlueFloorPurpleStripes" },
		{ 23, "BluePurpleFloorWithLeaf" },
		{ 24, "BlueFloorPurpleCorner" },
		{ 25, "BlueFloorGreenEdges" },
		{ 26, "BotLeftBigRock" },
		{ 27, "BotRightBigRock" },
		{ 28, "TopLeftBigRock" },
		{ 29, "TopRightBigRock" },
		{ 30, "WallCrumbleDown" },
		{ 31, "WallCrumbleRight" },
		{ 32, "LightBlueFloorBrick" },
		{ 33, "CornPlant" },
		{ 34, "BlueFloor" },
		{ 35, "LightBlueFloorBlueEdge" },
		{ 36, "BlueFloorLightBlueEdge" },
		{ 37, "LightBlueFloor" },
		{ 38, "GreenFloor" },
		{ 39, "BlueFloorBigGreenStripe" },
		{ 40, "BlueFloorSmallGreenStripe" },
		{ 41, "WaterUpperWall" },
		{ 42, "WaterStill" },
		{ 43, "WaterShiny" },
		{ 44, "BLANK" },
		{ 45, "GlassBottom" },
		{ 46, "GlassUp" },
		{ 47, "GlassUpRight" },
		{ 48, "GlassUpLeft" },
		{ 49, "GlassBotRight" },
		{ 50, "GlassBotLeft" }
	};

	if (tileNum > 50)
		return "Unknown";

	return tileNames.at(tileNum);
}
