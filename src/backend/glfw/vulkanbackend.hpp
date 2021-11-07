#ifndef GLFW_VULKAN_BACKEND_HPP
#define GLFW_VULKAN_BACKEND_HPP

#include <string>
#include <map>
#include <memory>
#include "windows/base/windowbase.hpp"
#include "common/utils.hpp"

class GLFWwindow;

namespace window {
namespace glfw {

class VulkanBackend {
public:
    VulkanBackend(int width, int height);
    ~VulkanBackend();

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