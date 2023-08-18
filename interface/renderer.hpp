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

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include "views/ProcessesViewer.hpp"
#include "views/MemoryViewer.hpp"
#include "views/StringsViewer.hpp"
#include "views/TopBar.hpp"

#include "style.h"

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

            // windows here
            ui::views::TopBar();
            ui::views::Processes();
            ui::views::MemoryViewer();
            //ui::views::StringsViewer();

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