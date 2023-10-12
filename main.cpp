/*
* Remap
* A memory editor, written in C++ and ImGui.
*/

#include "interface/renderer.h"

int main() {
    GLFWwindow* window;

    renderer::Initialize(window);
    renderer::Render(window);
    renderer::Terminate(window);

    return 0;
}