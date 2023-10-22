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
    Window(int width, int height, std::string_view title) {
        glfwInit();
        window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
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
    RenderWindow(int width, int height, std::string_view title) : Window(width, height, title) {
        ImGui::CreateContext();
        ImNodes::CreateContext();

        ApplyCustomStyle();

        ImGui_ImplGlfw_InitForOpenGL(get(), true);
        ImGui_ImplOpenGL3_Init("#version 130");
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

        views.push_back(std::make_unique<TopBar>());
        views.push_back(std::make_unique<ProcessesView>());
        views.push_back(std::make_unique<DisassemblyView>());
        views.push_back(std::make_unique<StringsView>());
        views.push_back(std::make_unique<ModulesView>());
        views.push_back(std::make_unique<ScriptingView>());
        views.push_back(std::make_unique<NodeEditor>());
    }

    virtual ~RenderWindow() {
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

            for (const auto& view : views) {
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
    std::vector<std::unique_ptr<View>> views;
};