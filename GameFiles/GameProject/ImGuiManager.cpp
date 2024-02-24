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

void ImGuiManager::RenderDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup)
{
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
		if (ImGui::Button("Top Right"))
		{
			// put your code here for mirror/emitter dir. set the direction based on button.
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Top Left"))
		{
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Bottom Right"))
		{
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Bottom Left"))
		{
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}