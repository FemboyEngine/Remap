/*
* Remap
* A memory editor, written in C++ and ImGui.
* With a focus on usability and performance.
*
* Like Cheat Engine, Reclass.
* With better User Interface
*
* This file is part of Remap.
* - views/views.h
*
* Header file for views namespace.
*/

#pragma once

namespace ui::views {
    void TopBar() noexcept;
    void Processes() noexcept;
    void MemoryViewer() noexcept;
    void StringsViewer() noexcept;
    void ModulesViewer() noexcept;
}