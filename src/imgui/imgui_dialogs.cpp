//
//
#include <thread>
#include <stdarg.h>

#include <string>
#include <vector>
#include <algorithm>

#include <nfd.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_dialogs.h>

IMGUI_DIALOGS_BEGIN

static void OpenFileThread(const char *name, const char *def_path, std::string *out, Result *res)
{
	nfdresult_t ret;
	nfdchar_t *out_path = NULL;

	ret = NFD_OpenDialog(NULL, def_path, &out_path);
	switch (ret) {
	case NFD_OKAY:
		*out = out_path;
		free(out_path);
		*res = Result_ok;
		break;
	case NFD_CANCEL:
		*res = Result_cancel;
		break;
	default:
	case NFD_ERROR:
		*res = Result_error;
		break;
	}
}

IMGUI_API Result OpenFile(const char *str_id, const char *def_path, std::string &out)
{
	static std::thread thread;
	static std::string path;
	static Result res = Result_none;

	if (!ImGui::IsPopupOpen(str_id)) {
		ImGui::OpenPopup(str_id);
		thread = std::thread(OpenFileThread, str_id, def_path, &path, &res);
		return Result_none;
	}

	if (!ImGui::BeginPopupModal(str_id, NULL,
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize))
		return Result_none;

	if (res != Result_none) {
		out = path;
		thread.join();
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();

	return res;
}

static void OpenFilesThread(const char *name, const char *def_path, PathSet *out, Result *res)
{
	int i;
	int n;
	nfdresult_t ret;
	nfdpathset_t out_paths;

	ret = NFD_OpenDialogMultiple(NULL, def_path, &out_paths);
	switch (ret) {
	case NFD_OKAY:
		n = NFD_PathSet_GetCount(&out_paths);
		for (i = 0; i < n; ++i)
			out->push_back(NFD_PathSet_GetPath(&out_paths, i));
		NFD_PathSet_Free(&out_paths);
		*res = Result_ok;
		break;
	case NFD_CANCEL:
		*res = Result_cancel;
		break;
	default:
	case NFD_ERROR:
		*res = Result_error;
		break;
	}
}

IMGUI_API Result OpenFiles(const char *str_id, const char *def_path, PathSet &out)
{
	static std::thread thread;
	static PathSet paths;
	static Result res = Result_none;

	if (!ImGui::IsPopupOpen(str_id)) {
		ImGui::OpenPopup(str_id);
		thread = std::thread(OpenFilesThread, str_id, def_path, &paths, &res);
		return Result_none;
	}

	if (!ImGui::BeginPopupModal(str_id, NULL,
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize))
		return Result_none;

	if (res != Result_none) {
		std::move(paths.begin(), paths.end(), out.begin());
		thread.join();
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();

	return res;
}

static void SaveFileThread(const char *name, const char *def_path, std::string *path, Result *res)
{
	nfdresult_t ret;
	nfdchar_t *out_path = NULL;

	ret = NFD_SaveDialog(NULL, def_path, &out_path);
	switch (ret) {
	case NFD_OKAY:
		*path = out_path;
		free(out_path);
		*res = Result_ok;
		break;
	case NFD_CANCEL:
		*res = Result_cancel;
		break;
	default:
	case NFD_ERROR:
		*res = Result_error;
		break;
	}
}

IMGUI_API Result SaveFile(const char *str_id, const char *def_path, std::string &out)
{
	static std::thread thread;
	static std::string path;
	static Result res = Result_none;

	if (!ImGui::IsPopupOpen(str_id)) {
		ImGui::OpenPopup(str_id);
		thread = std::thread(SaveFileThread, str_id, def_path, &path, &res);
		return Result_none;
	}

	if (!ImGui::BeginPopupModal(str_id, NULL,
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize))
		return Result_none;

	if (res != Result_none) {
		out = path;
		thread.join();
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();

	return res;
}

static void PickFolderThread(const char *name, const char *def_path, std::string *out, Result *res)
{
	nfdresult_t ret;
	nfdchar_t *out_path = NULL;

	ret = NFD_PickFolder(def_path, &out_path);
	switch (ret) {
	case NFD_OKAY:
		*out = out_path;
		free(out_path);
		*res = Result_ok;
		break;
	case NFD_CANCEL:
		*res = Result_cancel;
		break;
	default:
	case NFD_ERROR:
		*res = Result_error;
		break;
	}
}

IMGUI_API Result PickFolder(const char *str_id, const char *def_path, std::string &out)
{
	static std::thread thread;
	static std::string path;
	static Result res = Result_none;

	if (!ImGui::IsPopupOpen(str_id)) {
		ImGui::OpenPopup(str_id);
		thread = std::thread(PickFolderThread, str_id, def_path, &path, &res);
		return Result_none;
	}

	if (!ImGui::BeginPopupModal(str_id, NULL,
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize))
		return Result_none;

	if (res != Result_none) {
		out = path;
		thread.join();
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();

	return res;
}

IMGUI_API Result MessageBox(const char *str_id, MessageBoxType type, const char *fmt, ...)
{
	IM_ASSERT(str_id != NULL);

	if (!ImGui::IsPopupOpen(str_id))
		ImGui::OpenPopup(str_id);

	if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
		return Result_none;

	if (fmt != NULL) {
		va_list args;
		va_start(args, fmt);
		ImGui::BeginChild("", ImVec2(400, 120));
		ImGui::TextWrappedV(fmt, args);
		ImGui::EndChild();
		va_end(args);
	}

	ImGui::Separator();

	Result res = Result_none;

	switch (type) {
	case MessageBoxType_ok:
		if (ImGui::Button("OK", ImVec2(100, 0)))
			res = Result_ok;
		break;
	case MessageBoxType_yesno:
		if (ImGui::Button("Yes", ImVec2(100, 0)))
			res = Result_yes;
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(100, 0)))
			res = Result_no;
		break;
	case MessageBoxType_yesnocancel:
		if (ImGui::Button("Yes", ImVec2(100, 0)))
			res = Result_yes;
		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(100, 0)))
			res = Result_no;
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(100, 0)))
			res = Result_cancel;
		break;
	default:
		IM_ASSERT(false);
		break;
	}

	if (res != Result_none)
		ImGui::CloseCurrentPopup();

	ImGui::EndPopup();

	return res;
}

IMGUI_DIALOGS_END

