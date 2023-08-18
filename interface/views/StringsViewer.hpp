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

static char rBuffer[MAX_BUFFER_SIZE];
static char rSearchBuffer[MAX_SEARCH_BUFFER_SIZE];

MEMORY_BASIC_INFORMATION memInfo;
SIZE_T bytes_read;
SIZE_T rSize;
LPVOID address = NULL;
LPCVOID rBase = nullptr;

// vector to store the strings
std::vector<std::string> strings;

void ui::views::StringsViewer() noexcept {
    ImGui::Begin(
        "Strings Viewer",
        &state::isRunning,
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_HorizontalScrollbar
    );

    if (state::CurrentProcess == NULL && state::pid == 0) return;

    ImGui::Text("Process: %s", selected.c_str());
    ImGui::Text("PID: %d", state::pid);

    if (rBase == nullptr) {
        rBase = (LPCVOID)GetProcessBaseAddress(state::pid);
        rSize = sizeof(buffer);

        ReadProcessMemory(state::CurrentProcess, rBase, rBuffer, rSize, &bytes_read);
    }

    // Dump Strings from the process
    if (state::mapped_strings == false) {
        std::string str;
        for (int i = 0; i < bytes_read; i++) {
            if (rBuffer[i] != '\0') {
                str += rBuffer[i];
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