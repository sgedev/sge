//
//
#include <stdarg.h>
#include <imgui_utils.h>

namespace ImGui {
	IMGUI_API MessageBoxResult MessageBox(const char *str_id, MessageBoxType type, const char *fmt, ...)
	{
		IM_ASSERT(str_id != NULL);

		if (!ImGui::IsPopupOpen(str_id))
			ImGui::OpenPopup(str_id);

		if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
			return MessageBoxResult_none;

		if (fmt != NULL) {
			va_list args;
			va_start(args, fmt);
			ImGui::TextV(fmt, args);
			va_end(args);
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::Indent(100.0f);

		MessageBoxResult ret = MessageBoxResult_none;

		switch (type) {
		case MessageBoxType_ok:
			if (ImGui::Button("OK", ImVec2(100, 0)))
				ret = MessageBoxResult_ok;
			break;
		case MessageBoxType_yesnocancel:
			if (ImGui::Button("Cancel", ImVec2(100, 0)))
				ret = MessageBoxResult_cancel;
			ImGui::SameLine();
			/* passthrough */
		case MessageBoxType_yesno:
			if (ImGui::Button("Yes", ImVec2(100, 0)))
				ret = MessageBoxResult_yes;
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(100, 0)))
				ret = MessageBoxResult_no;
			break;
		default:
			IM_ASSERT(false);
			break;
		}

		if (ret != MessageBoxResult_none)
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();

		return ret;
	}

	IMGUI_API const char *OpenFileDialog(const char *str_id)
	{
		IM_ASSERT(str_id != NULL);

		if (!ImGui::IsPopupOpen(str_id))
			ImGui::OpenPopup(str_id);

		if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
			return NULL;

		// ImGui::CloseCurrentPopup();

		ImGui::EndPopup();

		return NULL;
	}

	IMGUI_API const char *SaveFileDialog(const char *str_id)
	{
		IM_ASSERT(str_id != NULL);

		if (!ImGui::IsPopupOpen(str_id))
			ImGui::OpenPopup(str_id);

		if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
			return NULL;

		// ImGui::CloseCurrentPopup();

		ImGui::EndPopup();

		return NULL;
	}

	IMGUI_API const char *SelectFolderDialog(const char *str_id)
	{
		IM_ASSERT(str_id != NULL);

		if (!ImGui::IsPopupOpen(str_id))
			ImGui::OpenPopup(str_id);

		if (!ImGui::BeginPopupModal(str_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
			return NULL;

		// ImGui::CloseCurrentPopup();

		ImGui::EndPopup();

		return NULL;
	}
}

