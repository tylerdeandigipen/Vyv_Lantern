#include "ImGuiManager.h"
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

int ImGuiManager::RenderMirrorDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
{
	int returnVal = 0;
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

		//1
		if (ImGui::Button("Top Left"))
		{
			returnVal = 1;
			ImGui::CloseCurrentPopup();
		}

		//2
		if (ImGui::Button("Top Right"))
		{
			// put your code here for mirror/emitter dir. set the direction based on button.
			ImGui::CloseCurrentPopup();
			returnVal = 2;
		}

		//3
		if (ImGui::Button("Bottom Left"))
		{
			ImGui::CloseCurrentPopup();
			returnVal = 3;
		}

		//4
		if (ImGui::Button("Bottom Right"))
		{
			ImGui::CloseCurrentPopup();
			returnVal = 4;
		}

		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::Text("Insert reflection direction.");

		std::vector<std::pair<int, int>> positions;

		static int xValue = 0;
		static int yValue = 0;

		ImGui::InputInt("X", &xValue);
		ImGui::InputInt("Y", &yValue);

		if (ImGui::Button("Confirm Placement..."))
		{
			positions.push_back(std::make_pair(xValue, yValue));
		}

		ImGui::EndPopup();
		return returnVal;
	}
}

int ImGuiManager::RenderEmitterDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
{
	int returnVal = 0;
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

		//1
		if (ImGui::Button("Up"))
		{
			returnVal = 1;
			ImGui::CloseCurrentPopup();
		}

		//2
		if (ImGui::Button("Down"))
		{
			// put your code here for mirror/emitter dir. set the direction based on button.
			ImGui::CloseCurrentPopup();
			returnVal = 2;
		}

		//3
		if (ImGui::Button("Right"))
		{
			ImGui::CloseCurrentPopup();
			returnVal = 3;
		}

		//4
		if (ImGui::Button("Left"))
		{
			ImGui::CloseCurrentPopup();
			returnVal = 4;
		}

		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::Text("Insert reflection direction.");

		std::vector<std::pair<int, int>> positions;

		static int xValue = 0;
		static int yValue = 0;

		ImGui::InputInt("X", &xValue);
		ImGui::InputInt("Y", &yValue);

		if (ImGui::Button("Confirm Placement..."))
		{
			positions.push_back(std::make_pair(xValue, yValue));
		}

		ImGui::EndPopup();
		return returnVal;
	}
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