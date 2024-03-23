#ifndef WINDOW_BASE_HPP
#define WINDOW_BASE_HPP

#include <tuple>
#include <string>
#include <map>
#include <imgui.h>
#include <functional>

namespace window {

enum WindowMode
{
    window = 0,
    fullscreen = 1,
    to_window = 2,
    to_fullscreen = 3,
};

class WindowBase
{
public:
    WindowBase() = default;
    virtual ~WindowBase() = default;

    virtual bool Draw() = 0;

protected:
    void HandleKeyboard()
    {
        for (auto&& [_, item] : _key_handler)
        {
            if (ImGui::IsKeyPressed(std::get<0>(item), false))
            {
                std::get<1>(item)();
            }
        }
    }

    using KeyHandlers = std::map<std::string, std::tuple<ImGuiKey, std::function<void()>>>;
    KeyHandlers _key_handler;
};

}   // namespace end

#endif  // WINDOW_BASE_HPP