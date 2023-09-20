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
    void Disassembly() noexcept;
    void Strings() noexcept;
    void Modules() noexcept;

    void NodeEditor() noexcept;
    void Scripting() noexcept;
}

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