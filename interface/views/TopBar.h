#pragma once

#include <imjson/imjson.h>
#include <interface/style.h>

void ui::views::TopBar() {
    ImGui::BeginMainMenuBar();

	/* Remove this after implementing the options */
	#pragma warning(push)
	#pragma warning(disable: 4390) // ';': empty controlled statement found

    if (ImGui::BeginMenu("File"))
    {
		if (ImGui::MenuItem("Open", "Ctrl+O"));
		if (ImGui::MenuItem("Save", "Ctrl+S"));
		if (ImGui::MenuItem("Import", "Ctrl+I"))
			LoadStyle();
		if (ImGui::MenuItem("Exit", "Alt+F4"));

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
		if (ImGui::MenuItem("Undo", "Ctrl+Z"));
		if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false));  // disabled
		ImGui::Separator();
		if (ImGui::MenuItem("Cut", "Ctrl+X"));
		if (ImGui::MenuItem("Copy", "Ctrl+C"));
		if (ImGui::MenuItem("Paste", "Ctrl+V"));
		ImGui::EndMenu();
	}

    if (ImGui::BeginMenu("View"))
    {
        for (auto& [key, value] : states::running) {
            if (ImGui::MenuItem(key.c_str())) {
				value = !value;
			}
		}

        ImGui::EndMenu();
    }

	if (ImGui::BeginMenu("Help"));

	#pragma warning(pop)
    ImGui::EndMainMenuBar();
}