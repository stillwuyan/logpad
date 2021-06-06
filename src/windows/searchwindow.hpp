#ifndef SEARCH_WINDOWN_HPP
#define SEARCH_WINDOWN_HPP

#include <imgui/imgui.h>
#include "framework/windowchild.hpp"

namespace window { namespace logpad {

class SearchWindow : public WindowChild
{
public:
    SearchWindow() = default;
    ~SearchWindow() = default;

    bool Show() final override
    {
        bool open = true;
        ImGui::ShowDemoWindow(&open);
        return open;
    }
};

}}

#endif // SEARCH_WINDOWN_HPP