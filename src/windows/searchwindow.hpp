#ifndef SEARCH_WINDOWN_HPP
#define SEARCH_WINDOWN_HPP

#include <vector>
#include <string>
#include <atomic>
#include <imgui/imgui.h>
#include "framework/windowchild.hpp"
#include "input/filefilter.hpp"

namespace window { namespace logpad {

class SearchWindow : public WindowChild
{
public:
    SearchWindow(const std::string& name, const std::string& file);
    ~SearchWindow() = default;

    void Show() final override;

private:
    FileFilter _filter;
    int _selected;
    std::atomic<bool> _searching;
    char _search_text[1024];
};

}}

#endif // SEARCH_WINDOWN_HPP