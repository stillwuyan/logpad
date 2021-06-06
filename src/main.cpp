// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "platform/glfw/glfwbackend.hpp"
#include "windows/mainwindow.hpp"

int main(int, char**) {
    std::string name("main");
    auto container = window::GLFWBackend(1000, 700);
    std::unique_ptr<window::WindowContainer> main(new window::logpad::MainWindow(name));
    container.Insert(name, main);
    container.Process();
    return 0;
}