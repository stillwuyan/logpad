// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#ifndef GLFW_GL3_BACKEND_HPP
#define GLFW_GL3_BACKEND_HPP

#include "backend/glfw/glfwbase.hpp"

class GLFWwindow;

namespace window {
namespace glfw {

class GL3Backend : public GLFWBase
{
public:
    GL3Backend(int x, int y, int width, int height);
    ~GL3Backend();
private:
    void Initialize();
    void Finalize();
    bool Process();

    int _interval;
};

} // namespace glfw
}  // namespace window

#endif  // GLFW_GL3_BACKEND_HPP