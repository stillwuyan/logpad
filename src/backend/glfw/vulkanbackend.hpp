#ifndef GLFW_VULKAN_BACKEND_HPP
#define GLFW_VULKAN_BACKEND_HPP

#include "backend/glfw/glfwbase.hpp"

class GLFWwindow;

namespace window {
namespace glfw {

class VulkanBackend : public GLFWBase {
public:
    VulkanBackend(int x, int y, int width, int height);
    ~VulkanBackend();
private:
    void Initialize();
    void Finalize();
    bool Process();
};

} // namespace glfw
}  // namespace window

#endif  // GLFW_GL3_BACKEND_HPP