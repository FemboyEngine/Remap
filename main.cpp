/*
* Remap
* A memory editor, written in C++ and ImGui.
*/

#include "interface/renderer.h"

int main() {
    GLFWwindow* window;

    renderer::Init(window);
    renderer::loop(window);
    renderer::clean(window);

    return 0;
}