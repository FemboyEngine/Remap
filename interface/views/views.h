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
    void TopBar();
    void Processes();
    void Disassembly();
    void Strings();
    void Modules();

    void NodeEditor();
    void Scripting();
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