#ifndef DEMO_WINDOWN_HPP
#define DEMO_WINDOWN_HPP

#include <imgui/imgui.h>
#include "framework/windowchild.hpp"

namespace window { namespace logpad {

class DemoWindow : public WindowChild
{
public:
    DemoWindow() = default;
    ~DemoWindow() = default;

    bool Show() final override
    {
        bool open = true;
        ImGui::ShowDemoWindow(&open);
        return open;
    }
};

}}

#endif // DEMO_WINDOWN_HPP