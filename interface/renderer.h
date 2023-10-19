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

/**
 * Window - A base class for creating and managing the application window.
 */
class Window {
public:
    Window(int width, int height, const std::string& title) {
        glfwInit();
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwMakeContextCurrent(window);
    }

    virtual ~Window() {
        glfwTerminate();
    }

    GLFWwindow* get() const { return window; }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

private:
    GLFWwindow* window;
};

/**
 * RenderWindow - A class for creating and managing the rendering window.
 */
class RenderWindow : public Window {
public:
    RenderWindow(int width, int height, const std::string& title) : Window(width, height, title) {
        ImGui::CreateContext();
        ImNodes::CreateContext();

        ApplyCustomStyle();

        ImGui_ImplGlfw_InitForOpenGL(get(), true);
        ImGui_ImplOpenGL3_Init("#version 130");
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

        views.push_back(new TopBar());
        views.push_back(new ProcessesView());
        views.push_back(new DisassemblyView());
        views.push_back(new StringsView());
        views.push_back(new ModulesView());
        views.push_back(new ScriptingView());
        views.push_back(new NodeEditor());
    }

    virtual ~RenderWindow() {
        for (auto view : views) {
            delete view;
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    /**
     * Render - Render the views within the window.
     */
    void Render() {
        while (!glfwWindowShouldClose(get())) {
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

            for (auto view : views) {
                view->Render();
            }

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(get());
        }
    }

    RenderWindow(const RenderWindow&) = delete;
    RenderWindow& operator=(const RenderWindow&) = delete;

private:
    std::vector<View*> views;
};