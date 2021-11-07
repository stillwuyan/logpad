#ifndef WINDOW_CHILD_HPP
#define WINDOW_CHILD_HPP

#include <functional>
#include <memory>
#include <map>
#include <string>
#include <tuple>
#include <imgui.h>
#include "common/utils.hpp"
#include "windows/base/windowbase.hpp"

namespace window {

class WindowChild : public WindowBase
{
public:
    WindowChild(const std::string name)
    : _name(name)
    , _flags(0)
    , _mode(WindowMode::window)
    {
        _key_handler = {
            {
                "Fullscreen",
                {
                    [](ImGuiIO&) {
                        return ImGui::IsKeyPressed(ImGuiKey_F12, false);
                    },
                    [this]() {
                        ChangeMode();
                    }
                }
            }
        };
    }

    virtual ~WindowChild() = default;

    virtual bool Draw() override
    {
        bool open = true;
        if (_mode == WindowMode::to_fullscreen || _mode == WindowMode::to_window)
        {
            ImGui::SetNextWindowPos(_curr_pos);
            ImGui::SetNextWindowSize(_curr_size);
            _mode = (_mode == WindowMode::to_fullscreen ? WindowMode::fullscreen : WindowMode::window);
        }

        ImGui::Begin(_name.c_str(), &open, _flags);
        Show();
        HandleKeyboard();
        ImGui::End();

        return open;
    }

    virtual void Resize(int width, int height) override
    {}

protected:
    using KeyHandlers = std::map<std::string, std::tuple<std::function<bool(ImGuiIO&)>, std::function<void()>>>;
    using ChildWindows = std::map<std::string, std::unique_ptr<WindowChild>>;

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

    void ChangeMode()
    {
        if (_mode == WindowMode::fullscreen)
        {
            _curr_pos = _prev_pos;
            _curr_size = _prev_size;
            _mode = WindowMode::to_window;
        }
        else if (_mode == WindowMode::window)
        {
            _prev_pos = ImGui::GetWindowPos();
            _prev_size = ImGui::GetWindowSize();

            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            _curr_pos = viewport->WorkPos;
            _curr_size = viewport->WorkSize;
            // _flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
            _mode = WindowMode::to_fullscreen;
        }
    }

    KeyHandlers _key_handler;
    const std::string _name;
    ImGuiWindowFlags _flags;
    WindowMode _mode;
    ImVec2 _prev_pos;
    ImVec2 _prev_size;
    ImVec2 _curr_pos;
    ImVec2 _curr_size;
};

} // namespace end

#endif  // WINDOW_CHILD_HPP