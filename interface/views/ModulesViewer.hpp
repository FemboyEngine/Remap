/*
* Remap
* A memory editor, written in C++ and ImGui.
* With a focus on usability and performance.
*
* This file is part of Remap.
* - views/ModulesViewer.hpp
*
* Code for loaded modules viewer window.
*/

#include <vector>
#include <TlHelp32.h>

// vector of modules
std::vector<std::string> modules;

void ui::views::ModulesViewer() noexcept
{
	ImGui::Begin(
		"Modules Viewer",
		&state::isRunning,
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_HorizontalScrollbar
	);
	if (state::CurrentProcess == NULL && state::pid == 0) return;

	ImGui::Text("Process: %s", selected.c_str());
	ImGui::Text("PID: %d", state::pid);

	if (modules.empty())
	{
		modules = GetLoadedModules(state::pid);
	}

	for (auto& module : modules)
	{
		ImGui::Text("%s", module.c_str());
	}

	ImGui::End();
}