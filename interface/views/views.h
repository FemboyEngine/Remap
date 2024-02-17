/*
* Header file for views class.
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

/**
 * View - A base class for creating and rendering views in Remap
 *
 * This class allows for the creation and rendering of various views within the application
 *
 * @param title: The title of the view
 * @param window: Indicates whether to create a window for the view
 * @param flag: A pointer to a flag that controls the visibility of the view
 */
class View {
public:
    View(const char* title, bool create_window = true, bool* visibility_flag = nullptr)
        : title(title), create_window(create_window), visibility_flag(visibility_flag) {}

    /**
     * Render - Render the view.
     *
     * This function renders the view based on its visibility flag
     * and whether it should be displayed in a window
     */
    void Render() {
        bool* flag = visibility_flag ?
            visibility_flag : &ui::views::states::running[std::string(title)];
        if (!*flag)
            return;

        if (create_window) {
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            if (ImGui::Begin(title, visibility_flag, 
                ImGuiWindowFlags_NoSavedSettings | 
                ImGuiWindowFlags_NoCollapse | 
                ImGuiWindowFlags_HorizontalScrollbar)) {

                Content();
                ImGui::End();
            }
        }
        else {
            Content();
        }
    }

protected:
    /**
     * This pure virtual function must be implemented
     * in derived classes to define the content of the view
     */
    virtual void Content() = 0;

private:
    const char* title;
    bool create_window;
    bool* visibility_flag;
};