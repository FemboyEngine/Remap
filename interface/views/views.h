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
 * @param window: Indicates whether to create a window for the view (default is true)
 * @param flag: A pointer to a flag that controls the visibility of the view (default is nullptr)
 */
class View {
public:
    View(std::string_view title, bool window = true /* Create Window? */, bool* flag = nullptr)
        : title(title), vflag(flag), window(window) {}

    virtual ~View() = default;

    /**
     * Render - Render the view.
     *
     * This function renders the view based on its visibility flag and whether it should be displayed in a window
     */
    void Render() {
        bool* flag = vflag ? vflag : &ui::views::states::running[std::string(title)];
        if (!*flag)
            return;

        if (window) {
            ImGui::Begin(
                title.data(),
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
    std::string_view title;
    bool window;
    bool* vflag;
};