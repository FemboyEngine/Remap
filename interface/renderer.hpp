/*
* Remap
* A memory editor, written in C++ and ImGui.
* With a focus on usability and performance.
*
* This file is part of Remap.
* - renderer.hpp
* 
* Code for rendering the interface.
*/
#define GL_CLAMP_TO_EDGE 0x812F

#define STB_IMAGE_IMPLEMENTATION
#include "imgui/stb_image.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include "views/ProcessesViewer.hpp"
#include "views/MemoryViewer.hpp"
#include "views/StringsViewer.hpp"
#include "views/ModulesViewer.hpp"
#include "views/TopBar.hpp"

#include "style.h"

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

namespace renderer {

    void Init(GLFWwindow*& window) {
        glfwInit();
        window = glfwCreateWindow(1280, 720, "Remap", NULL, NULL);
        glfwMakeContextCurrent(window);

        ImGui::CreateContext();

        ApplyCustomStyle();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    }

    void loop(GLFWwindow* window) {
        bool dirty = true;
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            if (ImGui::IsMousePosValid())
                dirty = true;

            if (!dirty)
                continue;

            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            ImGui::Begin("Background", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);

            static GLuint texture_id = 0;
            static int image_width = 0;
            static int image_height = 0;
            if (texture_id == 0)
				LoadTextureFromFile("C:\\Users\\Mole\\Desktop\\Pics\\thighs.jpg",
                    &texture_id, &image_width, &image_height);

            //ImGui::Begin("Thighs");
            //ImGui::Image((void*)texture_id, ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y));
            //ImGui::End();

            // windows here
            ui::views::TopBar();
            ui::views::Processes();
            //ui::views::MemoryViewer();
            //ui::views::StringsViewer();
            //ui::views::ModulesViewer();

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
    }

    void clean(GLFWwindow* window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
    }

}