#pragma once

#include <string>

class ImGuiManager
{
public:
	/* Displays a basic popup with an "OK" to close. For ALMOST every purpose, do not pass in a bool, or set it to false. */
	static void RenderOKPopup(const std::string& popupTitle, const std::string& message, bool openPopup = false);

	/* Displays directional popup, used for mirros and emitters. For ALMOST every purpose, do not pass in a bool, or set it to false. */
	static void RenderDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup = false);

private:
};