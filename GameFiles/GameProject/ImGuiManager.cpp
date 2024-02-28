#include "ImGuiManager.h"
#include <imgui.h>

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

int ImGuiManager::RenderDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
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
			ImGui::CloseCurrentPopup();
			returnVal = 1;
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

		ImGui::EndPopup();
		return returnVal;
	}
}