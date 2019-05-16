//
//
#ifndef IMGUI_UTILS_H
#define IMGUI_UTILS_H

#include <string>

#include <imgui.h>

namespace ImGui { namespace Utils {
	enum MessageBoxType {
		MessageBoxType_ok = 0,
		MessageBoxType_yesno,
		MessageBoxType_yesnocancel
	};

	enum Result {
		Result_none = 0,
		Result_ok,
		Result_yes,
		Result_no,
		Result_cancel
	};

	IMGUI_API Result OpenFileDialog(const char *str_id, std::string &path);
	//IMGUI_API Result SaveFileDialog(const char *str_id, std::string &path);
	//IMGUI_API Result SelectFolderDialog(const char *str_id, std::string &path);
	IMGUI_API Result MessageBox(const char *str_id, MessageBoxType type, const char *fmt, ...);
} }

#endif // IMGUI_UTILS_H

