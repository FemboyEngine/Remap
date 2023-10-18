/*
* Code for loaded modules viewer window.
* - views/Modules.h
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
	if (state::current_process == NULL && state::pid == 0) return;

	modules.clear();
	modules = remap::GetLoadedModules(state::pid);

	for (auto& module : modules)
	{
		ImGui::Text("%s", module.c_str());
	}

	ImGui::End();
}