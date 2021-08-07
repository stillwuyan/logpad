#ifndef WINDOW_CONTAINER_HPP
#define WINDOW_CONTAINER_HPP

#include <string>
#include <functional>
#include <memory>
#include <map>
#include <tuple>
#include <imgui/imgui.h>
#include "framework/windowbase.hpp"

namespace window {

class WindowContainer : public WindowBase
{
public:
    WindowContainer(const std::string& name)
    : _resize(false)
    , _width(0)
    , _height(0)
    , _name(name)
    {
        _flags = ImGuiWindowFlags_NoCollapse
               | ImGuiWindowFlags_NoResize
               | ImGuiWindowFlags_NoScrollbar
               | ImGuiWindowFlags_HorizontalScrollbar
               | ImGuiWindowFlags_NoTitleBar
               | ImGuiWindowFlags_NoBringToFrontOnFocus
               // | ImGuiWindowFlags_NoBackground
               | ImGuiWindowFlags_NoMove;
    }

    virtual ~WindowContainer() = default;

    virtual bool Draw() override
    {
        if (_resize)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(_width, _height), ImGuiCond_Always);
            _resize = false;
        }

        Show();

        std::erase_if(_child_windows, [](const auto& item) {
            bool ret = item.second->Draw();
            return !ret;
        });

        HandleKeyboard();

        return true;
    }

    virtual void Resize(int width, int height) override
    {
        if (_width != width || _height != height)
        {
            _resize = true;
            _width = width;
            _height = height;
        }
    }

protected:
    using KeyHandlers = std::map<std::string, std::tuple<std::function<bool(ImGuiIO&)>, std::function<void()>>>;
    using ChildWindows = std::map<std::string, std::unique_ptr<WindowBase>>;

    virtual void Show() = 0;

    void HandleKeyboard()
    {
        auto& io = ImGui::GetIO();
        for (auto&& [_, item] : _key_handler)
        {
            if (std::get<0>(item)(io))
            {
                std::get<1>(item)();
            }
        }
    }

    void UpdateChildWindow(std::string&& name, std::unique_ptr<WindowBase>&& child)
    {
        auto iter = _child_windows.find(name);
        if (iter != _child_windows.end())
        {
            _child_windows.erase(iter);
        }
        else
        {
            _child_windows.emplace(std::forward<std::string>(name), std::forward<decltype(child)>(child));
        }
    }

    ImGuiWindowFlags _flags;
    bool _resize;
    int _width;
    int _height;
    const std::string _name;

    KeyHandlers _key_handler;
    ChildWindows _child_windows;
};

}   // namespace end

#endif  // WINDOW_CONTAINER_HPP