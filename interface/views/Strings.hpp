/*
* Remap
* A memory editor, written in C++ and ImGui.
*
* This file is part of Remap.
* - views/processes.hpp
*
* Code for strings viewer window
*/

MEMORY_BASIC_INFORMATION memInfo;
LPVOID address = NULL;

std::vector<std::string> strings;

void ui::views::Strings() noexcept {

    if (!states::running["Strings"])
        return;

    ImGui::Begin(
        "Strings",
        &states::running["Strings"],
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_HorizontalScrollbar
    );

    if (state::CurrentProcess == NULL && state::pid == 0) return;

    if (state::mapped_strings == false) {
        strings.clear();

        std::string str;
        str.reserve(state::memory.size());
        strings.reserve(state::memory.size() / 10);
        for (auto it = state::memory.begin(); it != state::memory.end(); ++it) {
            if (isprint(*it)) {
                str.push_back(*it);
            }
            else {
                if (!str.empty()) {
                    strings.emplace_back(std::move(str));
                }
            }
        }
        if (!str.empty()) {
            strings.emplace_back(std::move(str));
        }
        state::mapped_strings = true;
    }

    if (state::mapped_strings) {
        ImGuiListClipper clipper;
        clipper.Begin(strings.size());

        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                ImGui::Text("%s", strings[i].c_str());
            }
        }
    }

    ImGui::End();
}