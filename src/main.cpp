// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#if defined(USE_GL3)
#include "backend/glfw/gl3backend.hpp"
#elif defined(USE_VULKAN)
#include "backend/glfw/vulkanbackend.hpp"
#endif
#include "windows/mainwindow.hpp"
#include "common/logger.hpp"

int main(int, char**) {
    ENTRY;
    std::string name("main");
#if defined(USE_GL3)
    LOG_INFO("Use {}", "OpenGL");
    auto backend = window::glfw::GL3Backend(200, 200, 1000, 700);
#elif defined(USE_VULKAN)
    LOG_INFO("Use {}", "Vulkan");
    auto backend = window::glfw::VulkanBackend(200, 200, 1000, 700);
#endif
    std::shared_ptr<window::WindowContainer> main = std::make_shared<window::logpad::MainWindow>(name);
    backend.SetWindowContainer(name, main);
    backend.Draw();
    return 0;
}