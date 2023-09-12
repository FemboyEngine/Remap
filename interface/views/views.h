/*
* Remap
* A memory editor, written in C++ and ImGui.
*
* This file is part of Remap.
* - views/views.h
*
* Header file for views namespace.
*/

#pragma once
#include <unordered_map>

namespace ui::views {
    void TopBar() noexcept;
    void Processes() noexcept;
    void MemoryViewer() noexcept;
    void StringsViewer() noexcept;
    void ModulesViewer() noexcept;

    void NodeEditor() noexcept;
    void ScriptingView() noexcept;
}

namespace ui::views::states {
    std::unordered_map<std::string, bool> running = {
		{ "Processes", false },
		{ "Memory Viewer", false },
		{ "Strings Viewer", false },
		{ "Modules Viewer", false },
		{ "Node Editor", false },
        { "Scripting View", false }
	};
}