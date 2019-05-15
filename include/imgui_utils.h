//
//
#ifndef IMGUI_UTILS_H
#define IMGUI_UTILS_H

#include <imgui.h>

namespace ImGui {
	enum MessageBoxType {
		MessageBoxType_ok = 0,
		MessageBoxType_yesno,
		MessageBoxType_yesnocancel
	};

	enum MessageBoxResult {
		MessageBoxResult_none = 0,
		MessageBoxResult_ok,
		MessageBoxResult_yes,
		MessageBoxResult_no,
		MessageBoxResult_cancel
	};

	IMGUI_API MessageBoxResult MessageBox(const char *str_id, MessageBoxType type, const char *fmt, ...);
	IMGUI_API const char *OpenFileDialog(const char *str_id);
	IMGUI_API const char *SaveFileDialog(const char *str_id);
	IMGUI_API const char *SelectFolderDialog(const char *str_id);
}

#endif // IMGUI_UTILS_H

