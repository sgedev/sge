//
//
#include <stdarg.h>

#include <string>
#include <vector>

#if defined(WIN32)
#elif defined(__linux__)
#	include <sys/types.h>
#	include <dirent.h>
#	include <unistd.h>
#else
#	error unknown os.
#endif

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_utils.h>

namespace ImGui { namespace Utils {
	enum Type {
		TYPE_UNKNOWN = 0,
		TYPE_DIR,
		TYPE_FILE
	};

	struct Entry {
		std::string name;
		Type type;
	};

	typedef std::vector<Entry> EntryListType;

	static EntryListType EntryList;

#if defined(WIN32)
#elif defined(__linux__)
	static bool UpdateEntryList(std::string &path)
	{
		int ret;
		DIR *dir;
		struct dirent *dent;

		if (path.empty()) {
			char cwd[PATH_MAX];
			if (getcwd(cwd, PATH_MAX) == NULL)
				return false;
			path = cwd;
		}

		dir = opendir(path.c_str());
		if (dir == NULL)
			return false;

		Entry ent;
		EntryList.clear();

		while ((dent = readdir(dir)) != NULL) {
			if (strcmp(dent->d_name, ".") == 0 ||
				strcmp(dent->d_name, "..") == 0)
				continue;
			switch (dent->d_type) {
			case DT_DIR:
				ent.name = "[";
				ent.name += dent->d_name;
				ent.name += "]";
				ent.type = TYPE_DIR;
				EntryList.push_back(ent);
				break;
			case DT_REG:
				ent.name = dent->d_name;
				ent.type = TYPE_FILE;
				EntryList.push_back(ent);
				break;
			}
		}

		closedir(dir);

		return true;
	}
#else
#	error unknown os.
#endif

	static bool EntryGetter(void* data, int idx, const char** out_text)
	{
		*out_text = EntryList[idx].name.c_str();
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
		ImGui::TextUnformatted(cur_path.c_str());
		ImGui::ListBox("", &sel, EntryGetter, NULL, EntryList.size());
		ImGui::Separator();

		Result res = Result_none;

		if (ImGui::Button("OK", ImVec2(100, 0)))
			res = Result_ok;

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

