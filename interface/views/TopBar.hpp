#ifndef TOPBAR_HPP
#define TOPBAR_HPP

void ui::views::TopBar() noexcept
{
    ImGui::BeginMainMenuBar();

	// does nothing yet
    if (ImGui::BeginMenu("File"))
    {
		if (ImGui::MenuItem("Open", "Ctrl+O"));
		if (ImGui::MenuItem("Save", "Ctrl+S"));
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
		if (ImGui::MenuItem("Processes", "Ctrl+P"));
		if (ImGui::MenuItem("Memory Viewer", "Ctrl+M"));
		if (ImGui::MenuItem("Top Bar", "Ctrl+T"));
		if (ImGui::MenuItem("Status Bar", "Ctrl+B"));
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"));

    ImGui::EndMainMenuBar();
}

#endif // TOPBAR_HPP