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

class Window {
public:
    Window(int width, int height, const std::string& title) {
        glfwInit();
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        glfwMakeContextCurrent(window);
    }

    ~Window() {
        glfwTerminate();
    }

    GLFWwindow* get() const { return window; }

private:
    GLFWwindow* window;
};

class RenderWindow : public Window {
public:
    RenderWindow(int width, int height, const std::string& title) : Window(width, height, title) {
        ImGui::CreateContext();
        ImNodes::CreateContext();

        ApplyCustomStyle();

        ImGui_ImplGlfw_InitForOpenGL(get(), true);
        ImGui_ImplOpenGL3_Init("#version 130");
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    }

    ~RenderWindow() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

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

            glfwSwapBuffers(get());
        }
    }
};

class View {
public:
    View(const std::string& title) : title(title) {}

    virtual ~View() {}

    void Render() {
        if (!ui::views::states::running[title])
            return;

        ImGui::Begin(
            title.c_str(),
            &ui::views::states::running[title],
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_HorizontalScrollbar |
            ImGuiWindowFlags_NoSavedSettings
        );

        Content();

        ImGui::End();
    }

protected:
    virtual void Content() = 0;

private:
    std::string title;
};