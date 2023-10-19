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

/**
 * View - A base class for creating and rendering views in Remap
 *
 * This class allows for the creation and rendering of various views within the application
 *
 * @param title: The title of the view
 * @param window: Indicates whether to create a window for the view (default is true)
 * @param flag: A pointer to a flag that controls the visibility of the view (default is nullptr)
 */
class View {
public:
    View(const std::string& title, bool window = true /* Create Window? */, bool* flag = nullptr)
        : title(title), e_flag(flag), window(window) {}

    virtual ~View() {}

    /**
     * Render - Render the view.
     *
     * This function renders the view based on its visibility flag and whether it should be displayed in a window
     */
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
    /**
     * Content - Define the content of the view.
     *
     * This pure virtual function must be implemented in derived classes to define the content of the view
     */
    virtual void Content() = 0;

private:
    std::string title;
    bool window;
    bool* e_flag;
};