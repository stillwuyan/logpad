#ifndef DEMO_WINDOWN_HPP
#define DEMO_WINDOWN_HPP

#include <imgui/imgui.h>
#include "framework/windowchild.hpp"

namespace window { namespace logpad {

class DemoWindow : public WindowChild
{
public:
    DemoWindow(const std::string name)
    : WindowChild(name)
    {}
    ~DemoWindow() = default;

    bool Draw() final override
    {
        bool open = true;
        ImGui::ShowDemoWindow(&open);
        return open;
    }

    void Show() final override {}
};

}}

#endif // DEMO_WINDOWN_HPP