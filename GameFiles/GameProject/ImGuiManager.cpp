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

static bool topLeftChecked = false;
static bool topRightChecked = false;
static bool bottomLeftChecked = false;
static bool bottomRightChecked = false;
static int returnVal = 0;
static MirrorData dontworry;
static MirrorData emptyThing;
MirrorData ImGuiManager::RenderMirrorDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
{
	static ImVec4 pickedColor = ImVec4(160.0 / 255.0f, 0.0f, 255.0f / 255.0f, 1.0f);
	returnVal = 0;
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
			dontworry.newcolor = { static_cast<uint8_t>(pickedColor.w), static_cast<uint8_t>(pickedColor.x), static_cast<uint8_t>(pickedColor.y), static_cast<uint8_t>(pickedColor.z) };
			dontworry.spriteDirection = returnVal;
			dontworry.done = true;
			// clean up
			returnVal = 0;
			topLeftChecked = false;
			topRightChecked = false;
			bottomLeftChecked = false;
			bottomRightChecked = false;
			ImGui::EndPopup();
			MirrorData worried(dontworry); // returns a copy so the values can be reset
			dontworry = MirrorData();
			return worried;
		}

		ImGui::EndPopup();
		return dontworry;
	}
	return emptyThing;
}

static bool upChecked = false;
static bool downChecked = false;
static bool rightChecked = false;
static bool leftChecked = false;
EmitterData ImGuiManager::RenderEmitterDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
{
	static EmitterData emitData;
	static EmitterData emptyEmit;
	returnVal = 0;
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
			emitData.done = true;
			emitData.spriteDirection = returnVal;
			emitData.newcolor = { static_cast<uint8_t>(pickedColor.x), static_cast<uint8_t>(pickedColor.y), static_cast<uint8_t>(pickedColor.z), static_cast<uint8_t>(pickedColor.w) };
			returnVal = 0;
			// clean up
			upChecked = false;
			downChecked = false;
			rightChecked = false;
			leftChecked = false;
			ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
			EmitterData hatethis(emitData); // returns a copy so the values can be reset
			emitData = EmitterData();
			return hatethis;
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
	return emptyEmit;
}