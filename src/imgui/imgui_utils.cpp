//
//
#include <stdarg.h>

#include <string>
#include <vector>
#include <algorithm>

#if defined(WIN32)
#elif defined(__linux__)
#	include <sys/types.h>
#	include <sys/stat.h>
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
	enum Type {
		TYPE_DIR = 0,
		TYPE_FILE
	};

	struct Entry {
		std::string name;
		Type type;
		int64_t size;
	};

	typedef std::vector<Entry> EntryListType;

	static EntryListType EntryList;

	static void AddDirEntry(const char *name)
	{
		if (strcmp(name, ".") == 0)
			return;

		Entry ent = { name, TYPE_DIR, -1 };
		EntryList.push_back(ent);
	}

	static void AddFileEntry(const char *name, int64_t size)
	{
		if (strcmp(name, ".") == 0)
			return;

		Entry ent = { name, TYPE_FILE, size };
		EntryList.push_back(ent);
	}

	namespace OS {
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

			while ((d = readdir(dir)) != NULL) {
				switch (d->d_type) {
				case DT_DIR:
					AddDirEntry(d->d_name);
					break;
				case DT_REG:
					struct stat st;
					int ret = lstat((path + '/' + d->d_name).c_str(), &st);
					AddFileEntry(d->d_name, !ret ? st.st_size : -1);
					break;
				}
			}

			closedir(dir);

			return true;
		}
#else
#	error unknown os.
#endif
	}

	static bool UpdateEntryList(std::string &path)
	{
		EntryList.clear();

		if (!OS::UpdateEntryList(path))
			return false;

		std::sort(EntryList.begin(), EntryList.end(), [](const Entry &a, const Entry &b) {
			if (a.type < b.type)
				return true;
			if (a.type > b.type)
				return false;
			return a.name < b.name;
		});

		return true;
	}

	IMGUI_API Result OpenFileDialog(const char *str_id, std::string &path)
	{
		static int sel = -1;
		static std::string cur_path = path;

		IM_ASSERT(str_id != NULL);

		if (!ImGui::IsPopupOpen(str_id)) {
			sel = -1;
			UpdateEntryList(cur_path);
			ImGui::OpenPopup(str_id);
		}

		if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
			return Result_none;

		ImGui::PushItemWidth(600.0f);

		if (ImGui::Button("Up")) {
			sel = -1;
			cur_path += OS::PathSP;
			cur_path += "..";
			UpdateEntryList(cur_path);
		}

		ImGui::Separator();

		if (EntryList[sel].type == TYPE_FILE)
			ImGui::Text("%s%c%s", cur_path.c_str(), OS::PathSP, EntryList[sel].name.c_str());
		else
			ImGui::TextUnformatted(cur_path.c_str());

		ImGui::BeginChild("filelist", ImVec2(600, 300));

		ImGui::Columns(3, "mycolumns"); // 4-ways, with border

        ImGui::Separator();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("Type"); ImGui::NextColumn();
        ImGui::Text("Size"); ImGui::NextColumn();
        ImGui::Separator();

		for (int i = 0; i < EntryList.size(); ++i) {
			Entry &ent = EntryList[i];
            if (ImGui::Selectable(ent.name.c_str(), sel == i, ImGuiSelectableFlags_SpanAllColumns)) {
                sel = i;
				// TODO
			}
			ImGui::NextColumn();
			switch (ent.type) {
			case TYPE_DIR:
				ImGui::TextUnformatted("DIR");
				break;
			case TYPE_FILE:
				ImGui::TextUnformatted("FILE");
				break;
			}
			ImGui::NextColumn();
			if (ent.size < 0)
				ImGui::TextUnformatted("");
			else
				ImGui::Text("%ld", ent.size);
			ImGui::NextColumn();
		}

		ImGui::EndChild();

		ImGui::Separator();

		Result res = Result_none;

		if (ImGui::Button("OK", ImVec2(100, 0))) {

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
#if 0
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
#endif
		return Result_none;
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

