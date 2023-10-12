/*
* Remap
* A memory editor, written in C++ and ImGui.
*
* This file is part of Remap.
* - renderer.hpp
* 
* Code for rendering the interface.
*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <string>
#include <vector>

#include "views/Processes.h"
#include "views/Disassembly.h"
#include "views/Strings.h"
#include "views/Modules.h"
#include "views/NodeEditor.h"
#include "views/Scripting.h"
#include "views/TopBar.h"

#include "style.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

namespace renderer {

    void Initialize(GLFWwindow*& window) {
        glfwInit();
        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Remap", NULL, NULL);
        glfwMakeContextCurrent(window);

        ImGui::CreateContext();
        ImNodes::CreateContext();

        ApplyCustomStyle();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    }

    void Render(GLFWwindow* window) {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            ImGui::Begin("Background", nullptr, 
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoSavedSettings
            );

            ui::views::TopBar();
            ui::views::Processes();
            ui::views::Disassembly();
            ui::views::Strings();
            ui::views::Modules();
            ui::views::NodeEditor();
            ui::views::Scripting();

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
    }

    void Terminate(GLFWwindow* window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
    }

}