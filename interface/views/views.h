/*
* Header file for views namespace.
* - views/views.h
*/

#pragma once

#include <unordered_map>

namespace ui::views::states {
    std::unordered_map<std::string, bool> running = {
        { "Processes", false },
        { "Disassembly", false },
        { "Strings", false },
        { "Modules", false },
        { "Node Editor", false },
        { "Scripting", false }
    };
}

class View {
public:
    View(const std::string& title, bool window = true /* Create Window? */, bool* flag = nullptr)
        : title(title), e_flag(flag), window(window) {}

    virtual ~View() {}

    void Render() {
        bool* flag = e_flag ? e_flag : &ui::views::states::running[title];
        if (!*flag)
            return;

        if (window) {
            ImGui::Begin(
                title.c_str(),
                flag,
                ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_HorizontalScrollbar
            );
        }

        Content();

        ImGui::End();
    }

protected:
    virtual void Content() = 0;

private:
    std::string title;
    bool window;
    bool* e_flag;
};