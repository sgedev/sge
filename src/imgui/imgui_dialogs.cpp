//
//
#include <stdarg.h>

#include <string>
#include <vector>
#include <algorithm>

#include <pfs.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_dialogs.h>

IMGUI_DIALOGS_BEGIN

class Root {
public:
	Root(const char *name)
		: m_name(name)
		, m_curr(-1)
	{
	}

public:
	bool SetCurrentDirectory(const char *cwd)
	{
		m_dirent_list.clear();

		pfs_dirent_t ent;
		pfs_dir_t *dir = pfs_dir_open(cwd);
		if (dir == NULL)
			return false;

		while (pfs_dir_read(dir, &ent) == 1)
			m_dirent_list.push_back(ent);

		pfs_dir_close(dir);

		m_cwd = cwd;

		if (!m_dirent_list.empty()) {
			m_curr = 0;
			std::sort(m_dirent_list.begin(), m_dirent_list.end(),
				[](const pfs_dirent_t &a, const pfs_dirent_t &b) {
					if (a.type < b.type)
						return true;
					if (a.type > b.type)
						return false;
					return (strcmp(a.name, b.name) < 0);
				});
		} else
			m_curr = -1;

		return true;
	}

	const char *CurrentDirectory(void) const
	{
		return m_cwd.c_str();
	}

	bool Draw(bool show_files)
	{
		ImGui::Columns(3, "mycolumns");

		ImGui::Separator();

		ImGui::Text("Name");
		ImGui::NextColumn();
		ImGui::Text("Type");
		ImGui::NextColumn();
		ImGui::Text("Size");
		ImGui::NextColumn();

		ImGui::Separator();

		bool ret = false;

		for (int i = 0; i < m_dirent_list.size(); ++i) {
			pfs_dirent_t &ent = m_dirent_list[i];
			if (!show_files && ent.type == PFS_DIRENT_TYPE_FILE)
				continue;
			if (strcmp(ent.name, ".") == 0)
				continue;
		    if (ImGui::Selectable(ent.name, m_curr == i, ImGuiSelectableFlags_SpanAllColumns)) {
		        m_curr = i;
				ret = true;
			}
			ImGui::NextColumn();
			if (ent.type == PFS_DIRENT_TYPE_DIR)
				ImGui::TextUnformatted("DIR");
			ImGui::NextColumn();
			if (ent.type == PFS_DIRENT_TYPE_FILE) {
				if (ent.size < 1024)
					ImGui::Text("%ldB", ent.size);
				else if (ent.size < 1024*1024)
					ImGui::Text("%.2fKB", ent.size / 1024.0);
				else if (ent.size < 1024*1024*1024)
					ImGui::Text("%.2fMB", ent.size / (1024.0*1024.0));
				else
					ImGui::Text("%.2fGB", ent.size / (1024.0*1024.0*1024.0));
			}
			ImGui::NextColumn();
		}

		return ret;
	}

	const pfs_dirent_t *CurrentDirent(void) const
	{
		if (0 <= m_curr && m_curr < m_dirent_list.size())
			return &m_dirent_list[m_curr];

		return NULL;
	}

	const char *Name(void) const
	{
		return m_name.c_str();
	}

private:
	typedef std::vector<pfs_dirent_t> DirentListType;

	std::string m_name;
	std::string m_cwd;
	DirentListType m_dirent_list;
	int m_curr;
};

class FileDialog {
public:
	FileDialog(void)
		: m_root_curr(-1)
	{
	}

public:
	Result Draw(const char *str_id, std::string &path, bool show_files)
	{
		char tmp[PFS_PATH_MAX];

		IM_ASSERT(str_id != NULL);

		if (!ImGui::IsPopupOpen(str_id)) {
			ImGui::OpenPopup(str_id);
			if (path.empty())
				pfs_get_curr_dir(tmp, PFS_PATH_MAX);
			else
				pfs_get_real_path(path.c_str(), tmp, PFS_PATH_MAX);
			m_curr_dir = tmp;
			Update(m_curr_dir.empty() ? NULL : m_curr_dir.c_str());
		}

		if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
			return Result_none;

		ImGui::PushItemWidth(600.0f);

		if (ImGui::Button("Up")) {
			m_curr_dir += PFS_PATH_SP;
			m_curr_dir += "..";
			pfs_get_real_path(m_curr_dir.c_str(), tmp, PFS_PATH_MAX);
			m_curr_dir = tmp;
			Update(m_curr_dir.c_str());
		}

		if (m_root_list.size() > 1) {
			ImGui::SameLine();
			bool ret = ImGui::Combo("", &m_root_curr, &FileDialog::RootGetter, this, m_root_list.size());
			if (ret)
				Update(m_root_list[m_root_curr].CurrentDirectory());
		}

		ImGui::Separator();

		const pfs_dirent_t *ent = CurrentDirent();
		if (ent != NULL && ent->type == PFS_DIRENT_TYPE_FILE)
			ImGui::Text("%s%s%s", m_curr_dir.c_str(), PFS_PATH_SP, ent->name);
		else
			ImGui::TextUnformatted(m_curr_dir.c_str());

		ImGui::Separator();

		ImGui::BeginChild("filelist", ImVec2(600, 300));

		Root *r = CurrentRoot();
		if (r != NULL && r->Draw(show_files)) {
			const pfs_dirent_t *ent = CurrentDirent();
			if (ent != NULL && ent->type == PFS_DIRENT_TYPE_DIR) {
				m_curr_dir += PFS_PATH_SP;
				m_curr_dir += ent->name;
				pfs_get_real_path(m_curr_dir.c_str(), tmp, PFS_PATH_MAX);
				m_curr_dir = tmp;
				Update(m_curr_dir.c_str());
			}
		}

		ImGui::EndChild();

		ImGui::Separator();

		Result res = Result_none;

		if (ImGui::Button("OK", ImVec2(100, 0))) {
			res = Result_ok;
			path = m_curr_dir;
			if (show_files) {
				const pfs_dirent_t *ent = CurrentDirent();
				puts("1");
				if (ent != NULL && ent->type == PFS_DIRENT_TYPE_FILE) {
					puts("2");
					path += PFS_PATH_SP;
					path += ent->name;
				}
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

private:
	bool Update(const char *path)
	{
		pfs_update_roots();

		m_root_list.clear();

		int n = pfs_get_root_count();
		for (int i = 0; i < n; ++i)
			m_root_list.emplace_back(pfs_get_root_name(i));

		m_root_curr = pfs_get_path_root(path);

		Root *r = CurrentRoot();
		if (r != NULL)
			r->SetCurrentDirectory(path);
	}

	Root *CurrentRoot(void)
	{
		if (0 <= m_root_curr && m_root_curr < m_root_list.size())
			return &m_root_list[m_root_curr];
		puts("null root");
		return NULL;
	}

	const pfs_dirent_t *CurrentDirent(void)
	{
		Root *r = CurrentRoot();
		if (r != NULL)
			return r->CurrentDirent();
		return NULL;
	}

	static bool RootGetter(void *data, int idx, const char **out_text)
	{
		FileDialog *_this = (FileDialog *)data;

		if (0 <= idx && idx < _this->m_root_list.size()) {
			*out_text = _this->m_root_list[idx].Name();
			return true;
		}

		return false;
	}

private:
	typedef std::vector<Root> RootListType;
	RootListType m_root_list;
	int m_root_curr;
	std::string m_curr_dir;
};

IMGUI_API Result OpenFile(const char *str_id, std::string &path)
{
	static FileDialog diag;

	return diag.Draw(str_id, path, true);
}

IMGUI_API Result SelectFolder(const char *str_id, std::string &path)
{
	static FileDialog diag;

	return diag.Draw(str_id, path, false);
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

