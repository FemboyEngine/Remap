/*
* Remap
* A memory editor, written in C++ and ImGui.
*
* This file is part of Remap.
* - views/Modules.h
*
* Code for loaded modules viewer window.
*/

#include <vector>
#include <TlHelp32.h>

std::vector<std::string> modules;

void ui::views::Modules() {
	if (!states::running["Modules"]) 
		return;

	ImGui::Begin(
		"Modules",
		&states::running["Modules"],
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoSavedSettings
	);
	if (state::CurrentProcess == NULL && state::pid == 0) return;

	modules.clear();
	modules = remap::GetLoadedModules(state::pid);

	for (auto& module : modules)
	{
		ImGui::Text("%s", module.c_str());
	}

	ImGui::End();
}