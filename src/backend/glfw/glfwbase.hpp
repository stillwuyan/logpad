#ifndef GLFW_BASE_HPP
#define GLFW_BASE_HPP

#include "base/windowbase.hpp"
#include "base/windowcontainer.hpp"
#include "common/logger.hpp"
#include <GLFW/glfw3.h>

class GLFWwindow;

namespace window {
namespace glfw {

class GLFWBase : public WindowBase {
public:
    GLFWBase(int x, int y, int width, int height)
        : _name("Log Viewer")
        , _current_mode(WindowMode::window)
        , _window_pos{ {x, y, width, height}, {0, 0, 0, 0} }
        , _region("en")
        , _font_size(28.0f)
        , _window(nullptr)
    {
        _key_handler = {
            {
                "Fullscreen",
                {
                    ImGuiKey_F11,
                    [this]() {
                        ChangeMode();
                    },
                },
            },
        };
    }
    virtual ~GLFWBase() = default;

    void SetWindowContainer(const std::string& name, std::shared_ptr<WindowContainer>& window)
    {
        _container = window;
    }

    bool Draw()
    {
        Initialize();
        if (_window == nullptr)
        {
            return false;
        }
        return Process();
    }

protected:
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual bool Process() = 0;

    static void ErrorCallback(int error, const char* description)
    {
        LOG_ERR("Glfw Error {}: {}", error, description);
    }
    static void SizeCallback(GLFWwindow* window, int width, int height)
    {
        GLFWBase* instance = reinterpret_cast<GLFWBase*>(glfwGetWindowUserPointer(window));
        if (instance)
        {
            instance->OnSize(width, height);
        }
    }
    static void PosCallback(GLFWwindow* window, int x, int y)
    {
        GLFWBase* instance = reinterpret_cast<GLFWBase*>(glfwGetWindowUserPointer(window));
        if (instance)
        {
            instance->_window_pos[instance->_current_mode].x = x;
            instance->_window_pos[instance->_current_mode].y = y;
        }
    }

    void CreateWindow()
    {
        if (_current_mode == WindowMode::fullscreen)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
            _window_pos[WindowMode::fullscreen].w = vidmode->width;
            _window_pos[WindowMode::fullscreen].h = vidmode->height;
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            _window = glfwCreateWindow(vidmode->width, vidmode->height, _name.c_str(), nullptr, nullptr);
        }
        else
        {
            _window = glfwCreateWindow(_window_pos[_current_mode].w, _window_pos[_current_mode].h, _name.c_str(), nullptr, nullptr);
            glfwSetWindowPos(_window, _window_pos[_current_mode].x, _window_pos[_current_mode].y);
        }
    }
    
    void SetupWindow()
    {
        glfwSetWindowUserPointer(_window, this);
        glfwSetWindowSizeCallback(_window, GLFWBase::SizeCallback);
        glfwSetWindowPosCallback(_window, GLFWBase::PosCallback);
        OnSize(_window_pos[_current_mode].w, _window_pos[_current_mode].h);
    }

    void ChangeMode()
    {
        if (_current_mode == WindowMode::window)
        {
            _current_mode = WindowMode::fullscreen; // Must set _current_mode first, then OnSize() is invoked
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(_window, monitor,
                _window_pos[WindowMode::fullscreen].x,
                _window_pos[WindowMode::fullscreen].y,
                mode->width,
                mode->height,
                mode->refreshRate);
        }
        else if (_current_mode == WindowMode::fullscreen)
        {
            _current_mode = WindowMode::window; // Must set _current_mode first, then OnSize() is invoked
            glfwSetWindowMonitor(_window, nullptr,
                _window_pos[WindowMode::window].x,
                _window_pos[WindowMode::window].y,
                _window_pos[WindowMode::window].w,
                _window_pos[WindowMode::window].h,
                60);
        }
    }

    void OnSize(int width, int height)
    {
        _window_pos[_current_mode].w = width;
        _window_pos[_current_mode].h = height;
        _container->Resize(width, height);
    }

    struct WindowPos {
        int x;
        int y;
        int w;
        int h;
    };
    std::string _name;
    WindowMode _current_mode;
    WindowPos _window_pos[2];
    std::string _region;
    float _font_size;
    std::shared_ptr<WindowContainer> _container;
    GLFWwindow* _window;
};

} // namespace glfw
}  // namespace window

#endif  // GLFW_BASE_HPP