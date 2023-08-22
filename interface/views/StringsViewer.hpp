/*
* Remap
* A memory editor, written in C++ and ImGui.
* With a focus on usability and performance.
*
* This file is part of Remap.
* - views/processes.hpp
*
* Code for strings viewer window
*/

MEMORY_BASIC_INFORMATION memInfo;
LPVOID address = NULL;

// vector to store the strings
std::vector<std::string> strings;

void ui::views::StringsViewer() noexcept {

    if (!states::running["Strings Viewer"])
        return;

    ImGui::Begin(
        "Strings Viewer",
        &states::running["Strings Viewer"],
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_HorizontalScrollbar
    );

    if (state::CurrentProcess == NULL && state::pid == 0) return;

    ImGui::Text("Process: %s", selected.c_str());
    ImGui::Text("PID: %d", state::pid);

    if (state::mapped_strings == false) {
        std::string str;
        str.reserve(state::memory.size()); // pre allocate memory for string
        for (auto it = state::memory.begin(); it != state::memory.end(); ++it) {
            if (isprint(*it)) {
                str.push_back(*it);
            }
            else {
                if (!str.empty()) {
                    strings.push_back(str);
                    str.clear();
                }
            }
        }
        if (!str.empty()) {
            strings.push_back(str);
            str.clear();
        }
        state::mapped_strings = true;
    }

    if (state::mapped_strings) {
        for (int i = 0; i < strings.size(); i++) {
            ImGui::Text("%s", strings[i].c_str());
        }
    }

    ImGui::End();
}