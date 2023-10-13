/*
* Remap
* A memory editor, written in C++ and ImGui.
*
* This file is part of Remap.
* - views/Strings.h
*
* Code for strings viewer window
*/

MEMORY_BASIC_INFORMATION memInfo;
LPVOID address = NULL;

std::vector<std::string> strings;

void ui::views::Strings() {

    if (!states::running["Strings"])
        return;

    ImGui::Begin(
        "Strings",
        &states::running["Strings"],
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_HorizontalScrollbar |
        ImGuiWindowFlags_NoSavedSettings
    );

    if (state::CurrentProcess == NULL && state::pid == 0) return;

    if (!state::mapped_strings) {
        strings.clear();

        std::stringstream ss;
        for (const auto& ch : state::memory) {
            if (ch == '\n') {
                if (!ss.str().empty()) {
                    strings.push_back(ss.str());
                    ss.str(std::string());
                }
            }
            else if (isprint(ch)) {
                ss << ch;
            }
        }
        if (!ss.str().empty()) {
            strings.push_back(ss.str());
        }

        state::mapped_strings = true;
    }

    if (state::mapped_strings) {
        ImGuiListClipper clipper;
        clipper.Begin(static_cast<int>(strings.size()));

        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                ImGui::Text("%s", strings[i].c_str());
            }
        }
    }

    ImGui::End();
}