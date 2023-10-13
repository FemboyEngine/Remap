/*
* Remap
* A Memory alteration utility.
*/

#include "interface/renderer.h"

int main() {
    GLFWwindow* window;

    renderer::Initialize(window);
    renderer::Render(window);
    renderer::Terminate(window);

    return 0;
}