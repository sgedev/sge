//
//
#include <stdarg.h>

#include <string>
#include <vector>
#include <algorithm>

#if defined(WIN32)
#elif defined(__linux__)
#	include <sys/types.h>
#	include <dirent.h>
#	include <unistd.h>
#	include <limits.h>
#	include <stdlib.h>
#else
#	error unknown os.
#endif

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_utils.h>

namespace ImGui { namespace Utils {
	typedef std::vector<std::string> EntryListType;
	static EntryListType EntryList;

#if defined(WIN32)
	static const char PathSP = '\\';
	static bool UpdateEntryList(std::string &path)
	{
		EntryList.clear();
		return false;
	}
#elif defined(__linux__)
	static const char PathSP = '/';
	static bool UpdateEntryList(std::string &path)
	{
		int ret;
		DIR *dir;
		struct dirent *d;
		char tmp[PATH_MAX];

		if (path.empty()) {
			if (getcwd(tmp, PATH_MAX) == NULL)
				return false;
			path = tmp;
		}

		if (realpath(path.c_str(), tmp) != NULL)
			path = tmp;

		dir = opendir(path.c_str());
		if (dir == NULL)
			return false;

		std::string name;
		EntryList.clear();

		while ((d = readdir(dir)) != NULL) {
			if (strcmp(d->d_name, ".") == 0)
				continue;
			switch (d->d_type) {
			case DT_DIR:
				name = "0[DIR]  ";
				name += d->d_name;
				EntryList.push_back(name);
				break;
			case DT_REG:
				name = "1[FILE] ";
				name += d->d_name;
				EntryList.push_back(name);
				break;
			}
		}

		std::sort(EntryList.begin(), EntryList.end(), [](std::string &a, std::string &b) {
			return a < b;
		});

		closedir(dir);

		return true;
	}
#else
#	error unknown os.
#endif

	static bool EntryGetter(void *data, int idx, const char **out_text)
	{
		*out_text = EntryList[idx].c_str() + 1;
		return true;
	}

	IMGUI_API Result OpenFileDialog(const char *str_id, std::string &path)
	{
		static int sel = 0;
		static std::string cur_path = path;

		IM_ASSERT(str_id != NULL);

		if (!ImGui::IsPopupOpen(str_id)) {
			sel = 0;
			UpdateEntryList(cur_path);
			ImGui::OpenPopup(str_id);
		}

		if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
			return Result_none;

		ImGui::PushItemWidth(600.0f);

		if (ImGui::Button("Up")) {
			sel = 0;
			cur_path += PathSP;
			cur_path += "..";
			UpdateEntryList(cur_path);
		}

		ImGui::SameLine();

		if (EntryList[sel][0] == '1')
			ImGui::Text("%s%c%s", cur_path.c_str(), PathSP, EntryList[sel].c_str() + 8);
		else
			ImGui::TextUnformatted(cur_path.c_str());

		if (ImGui::ListBox("", &sel, EntryGetter, NULL, EntryList.size())) {
			const char *n = EntryList[sel].c_str();
			if (*n == '0') {
				sel = 0;
				cur_path += PathSP;
				cur_path += n + 8;
				UpdateEntryList(cur_path);
			}
		}

		ImGui::Separator();

		Result res = Result_none;

		if (ImGui::Button("OK", ImVec2(100, 0))) {
			const char *n = EntryList[sel].c_str();
			if (*n == '0') {
				cur_path += PathSP;
				cur_path += n + 8;
				UpdateEntryList(cur_path);
			} else {
				path = cur_path + PathSP + (EntryList[sel].c_str() + 8);
				res = Result_ok;
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(100, 0)))
			res = Result_cancel;

		if (res != Result_none)
			ImGui::CloseCurrentPopup();

		ImGui::PopItemWidth();
		ImGui::EndPopup();

		return res;
	}

	IMGUI_API Result SelectFolderDialog(const char *str_id, std::string &path)
	{
		static int sel = 0;
		static std::string cur_path = path;

		IM_ASSERT(str_id != NULL);

		if (!ImGui::IsPopupOpen(str_id)) {
			sel = 0;
			UpdateEntryList(cur_path);
			ImGui::OpenPopup(str_id);
		}

		if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
			return Result_none;

		ImGui::PushItemWidth(600.0f);

		if (ImGui::Button("Up")) {
			sel = 0;
			cur_path += PathSP;
			cur_path += "..";
			UpdateEntryList(cur_path);
		}

		ImGui::SameLine();
		ImGui::TextUnformatted(cur_path.c_str());
		ImGui::Separator();

		if (ImGui::ListBox("", &sel, EntryGetter, NULL, EntryList.size())) {
			const char *n = EntryList[sel].c_str();
			if (*n == '0') {
				sel = 0;
				cur_path += PathSP;
				cur_path += n + 8;
				UpdateEntryList(cur_path);
			}
		}

		ImGui::Separator();

		Result res = Result_none;

		if (ImGui::Button("OK", ImVec2(100, 0))) {
			path = cur_path;
			res = Result_ok;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(100, 0)))
			res = Result_cancel;

		if (res != Result_none)
			ImGui::CloseCurrentPopup();

		ImGui::PopItemWidth();
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
} }

