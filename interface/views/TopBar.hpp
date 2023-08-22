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
		if (ImGui::MenuItem("Processes", "Ctrl+P"))
			states::running["Processes"] = true;
		if (ImGui::MenuItem("Memory Viewer", "Ctrl+M"))
			states::running["Memory Viewer"] = true;
		if (ImGui::MenuItem("Strings Viewer", "Ctrl+T"))
			states::running["Strings Viewer"] = true;
		if (ImGui::MenuItem("Modules Viewer", "Ctrl+L"))
			states::running["Modules Viewer"] = true;
		if (ImGui::MenuItem("Node Editor", "Ctrl+N"))
			states::running["Node Editor"] = true;
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Help"));

    ImGui::EndMainMenuBar();
}

#endif // TOPBAR_HPP