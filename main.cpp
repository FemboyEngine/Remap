/*
* Remap
* A memory editor, written in C++ and ImGui.
* With a focus on usability and performance.
*
*/

#include <interface/renderer.hpp>

int main() {
    GLFWwindow* window;

    renderer::Init(window);
    renderer::loop(window);
    renderer::clean(window);

    return 0;
}