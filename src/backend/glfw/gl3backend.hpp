// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#ifndef GLFW_GL3_BACKEND_HPP
#define GLFW_GL3_BACKEND_HPP

#include <string>
#include <map>
#include <memory>
#include "windows/base/windowbase.hpp"
#include "common/utils.hpp"

class GLFWwindow;

namespace window {
namespace glfw {

class GL3Backend
{
public:
    GL3Backend(int width, int height);
    ~GL3Backend();

    void Insert(const std::string& name, std::unique_ptr<WindowBase>& child);
    void Remove(const std::string& name);
    void Process();

    static void ErrorCallback(int error, const char* description);
    static void SizeCallback(GLFWwindow* window, int width, int height);

private:
    void Initialize();
    void Finalize();
    void ChangeMode();
    void OnSize(int width, int height);

    GLFWwindow* _window;
    WindowMode _mode;
    int _width[2];
    int _height[2];
    int _interval;
    float _en_font_size;
    std::string _name;
    std::map<std::string, std::unique_ptr<WindowBase>> _child_windows;
};

} // namespace glfw
}  // namespace window

#endif  // GLFW_GL3_BACKEND_HPP