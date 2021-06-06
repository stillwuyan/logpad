// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#ifndef GLFW_BACKEND_HPP
#define GLFW_BACKEND_HPP

#include <string>
#include <map>
#include <memory>
#include "framework/windowcontainer.hpp"

class GLFWwindow;

namespace window {

enum WindowMode
{
    window = 0,
    fullscreen = 1,
};

class GLFWBackend
{
public:
    GLFWBackend(int width, int height);
    ~GLFWBackend();

    void Insert(const std::string& name, std::unique_ptr<WindowContainer>& child);
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
    std::map<std::string, std::unique_ptr<WindowContainer>> _child_windows;
};

}  // namespace end

#endif  // GLFW_CONTAINER_HPP