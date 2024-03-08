#include "ImGuiManager.h"
#include "LevelCreatorScene.h"
#include <string.h>
#include <imgui.h>
#include <cassert>

void ImGuiManager::RenderOKPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
{
	if (openPopup)
	{
		ImGui::OpenPopup(popupTitle.c_str());
		openPopup = false;
	}

	if (ImGui::BeginPopupModal(popupTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(message.c_str());
		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

MirrorData ImGuiManager::RenderMirrorDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
{
    MirrorData dontworry;
	int returnVal = 0;
	static ImVec4 pickedColor = ImVec4(160.0f / 255.0f, 0.0f, 255.0f / 255.0f, 1.0f);

	if (openPopup)
	{
		ImGui::OpenPopup(popupTitle.c_str());
		openPopup = false;
	}

	if (ImGui::BeginPopupModal(popupTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(message.c_str());
		ImGui::Text("Warning, this cannot be changed.");

		ImGui::Text(message.c_str());

		bool topLeftChecked = false;
		bool topRightChecked = false;
		bool bottomLeftChecked = false;
		bool bottomRightChecked = false;

		// 1
		ImGui::Checkbox("Top Left", &topLeftChecked);
		if (topLeftChecked)
		{
			returnVal = 1;
			//ImGui::CloseCurrentPopup();
		}

		// 2
		ImGui::Checkbox("Top Right", &topRightChecked);
		if (topRightChecked)
		{
			// put your code here for mirror/emitter dir. set the direction based on button.
			//ImGui::CloseCurrentPopup();
			returnVal = 2;

			//ImGui::CloseCurrentPopup();
		}

		// 3
		ImGui::Checkbox("Bottom Left", &bottomLeftChecked);
		if (bottomLeftChecked)
		{
			returnVal = 3;

			//ImGui::CloseCurrentPopup();
		}

		// 4
		ImGui::Checkbox("Bottom Right", &bottomRightChecked);
		if (bottomRightChecked)
		{
			returnVal = 4;

			//ImGui::CloseCurrentPopup();
		}

		ImGui::Separator();

		ImGui::ColorPicker4("Pick a Color", (float*)&pickedColor);

		ImGui::Text("Insert reflection direction.");
		dontworry.spriteDirection = returnVal;
		//std::vector<std::pair<int, int>> positions;

		static float xValue = 0;
		static float yValue = 0;

		ImGui::InputFloat("X", &xValue);
		ImGui::InputFloat("Y", &yValue);

		if (ImGui::Button("Confirm Placement..."))
		{
			//positions.push_back(std::make_pair(xValue, yValue));
			dontworry.direction = { xValue, yValue };
		}

		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
		return dontworry;
	}
	return dontworry;
}

EmitterData ImGuiManager::RenderEmitterDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
{
	int returnVal = 0;
	EmitterData emitData;
	static ImVec4 pickedColor = ImVec4(160.0f / 255.0f, 0.0f, 255.0f / 255.0f, 1.0f);

	if (openPopup)
	{
		ImGui::OpenPopup(popupTitle.c_str());
		openPopup = false;
	}

	if (ImGui::BeginPopupModal(popupTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(message.c_str());
		ImGui::Text("Warning, this cannot be changed.");

		ImGui::Text(message.c_str());

		bool upChecked = false;
		bool downChecked = false;
		bool rightChecked = false;
		bool leftChecked = false;

		// 1
		ImGui::Checkbox("Up", &upChecked);
		if (upChecked)
		{
			returnVal = 1;

			//ImGui::CloseCurrentPopup();
		}

		// 2
		ImGui::Checkbox("Down", &downChecked);
		if (downChecked)
		{
			// put your code here for mirror/emitter dir. set the direction based on button.
			returnVal = 2;

			//ImGui::CloseCurrentPopup();
		}

		// 3
		ImGui::Checkbox("Right", &rightChecked);
		if (rightChecked)
		{
			returnVal = 3;

			//ImGui::CloseCurrentPopup();
		}

		// 4
		ImGui::Checkbox("Left", &leftChecked);
		if (leftChecked)
		{
			returnVal = 4;

			//ImGui::CloseCurrentPopup();
		}

		ImGui::Separator();

		ImGui::ColorPicker4("Pick a Color", (float*)&pickedColor);

		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
			return emitData;
		}

		/*ImGui::Text("Insert reflection direction.");

		std::vector<std::pair<int, int>> positions;

		static int xValue = 0;
		static int yValue = 0;

		ImGui::InputInt("X", &xValue);
		ImGui::InputInt("Y", &yValue);

		if (ImGui::Button("Confirm Placement..."))
		{
			positions.push_back(std::make_pair(xValue, yValue));
		}*/

		ImGui::EndPopup();
		return emitData;
	}
	return emitData;
}

/*void ImGuiManager::MirrorCheckBox(int dir)
{
	ImGui::OpenPopup("MirroReflectPopUp");
	if (ImGui::BeginPopupModal("MirroReflectPopUp", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Pick a reflect direction.");

		if (dir == 1)
		{
			ImGui::Text("TOP LEFT SELECTED");

			//ImGui::Checkbox("Up", );
			//ImGui::Checkbox("Left");
			if (ImGui::Button("OK"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		else if (dir == 2)
		{
			ImGui::Text("TOP RIGHT SELECTED");

			//ImGui::Checkbox("Up", );
			//ImGui::Checkbox("Right");
		}
		else if (dir == 3)
		{
			ImGui::Text("BOTTOM LEFT SELECTED");

			//ImGui::Checkbox("Down", );
			//ImGui::Checkbox("Left");
		}
		else if (dir == 4)
		{
			ImGui::Text("BOTTOM RIGHT SELECTED");

			//ImGui::Checkbox("Down", );
			//ImGui::Checkbox("Right");
		}
		else
		{
			assert("What did you do wrong?");
		}
	}
}*/