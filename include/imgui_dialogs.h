//
//
#ifndef IMGUI_DIALOGS_H
#define IMGUI_DIALOGS_H

#include <string>
#include <vector>

#include <imgui.h>

#define IMGUI_DIALOGS_BEGIN namespace ImDialogs {
#define IMGUI_DIALOGS_END }

IMGUI_DIALOGS_BEGIN

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
	Result_cancel,
	Result_error
};

typedef std::vector<std::string> PathSet;

IMGUI_API Result OpenFile(const char *str_id, const char *def_path, std::string &out);
IMGUI_API Result OpenFiles(const char *str_id, const char *def_path, PathSet &out);
IMGUI_API Result SaveFile(const char *str_id, const char *def_path, std::string &out);
IMGUI_API Result PickFolder(const char *str_id, const char *def_path, std::string &out);
IMGUI_API Result MessageBox(const char *str_id, MessageBoxType type, const char *fmt, ...);

IMGUI_DIALOGS_END

#endif // IMGUI_DIALOGS_H

