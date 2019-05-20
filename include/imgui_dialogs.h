//
//
#ifndef IMGUI_DIALOGS_H
#define IMGUI_DIALOGS_H

#include <string>

#include <imgui.h>

#define IMGUI_DIALOGS_BEGIN namespace ImGui { namespace Dialogs {
#define IMGUI_DIALOGS_END } }

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
	Result_cancel
};

IMGUI_API Result OpenFile(const char *str_id, std::string &path);
//IMGUI_API Result SaveFile(const char *str_id, std::string &path);
IMGUI_API Result SelectFolder(const char *str_id, std::string &path);
IMGUI_API Result MessageBox(const char *str_id, MessageBoxType type, const char *fmt, ...);

IMGUI_DIALOGS_END

#endif // IMGUI_DIALOGS_H

