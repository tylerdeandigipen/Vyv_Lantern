#pragma once

#include <string>
#include "Entity.h"

class ImGuiManager
{
public:
	/* Displays a basic popup with an "OK" to close. For ALMOST every purpose, do not pass in a bool, or set it to false. */
	static void RenderOKPopup(const std::string& popupTitle, const std::string& message, bool openPopup = false);

	/* Displays directional popup, used for mirros and emitters. For ALMOST every purpose, do not pass in a bool, or set it to false.
	* RETURN: 1 - top left, 2, top  right, 3 bottom left, 4 bottom right */
	static int RenderMirrorDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup = false);
	static int RenderEmitterDirPopup(const std::string& popupTitle, const std::string& message, bool openPopup = false);

	static void MirrorCheckBox(int dir);

private:
};