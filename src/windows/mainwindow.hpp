#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <vector>
#include <string>
#include <functional>
#include "framework/windowbase.hpp"
#include "imgui.h"

namespace window { namespace logpad {

class MainWindow : public WindowBase
{
public:
    MainWindow(const std::string& name);
    ~MainWindow() = default;

    void Draw() final override;
    void Resize(int width, int height) final override;

private:
    void HandleKeyboard();
    bool ShowDemo();
    bool ShowSearch();

    int _selected;
    int _width;
    int _height;
    bool _resize;
    const std::string _name;
    ImGuiWindowFlags _flags;
    std::vector<std::string> _data;
    size_t _lines_count;
    std::vector<std::tuple<int, bool, std::function<bool(MainWindow*)>>> _show_window;
};

}}  // namespace end

#endif  // MAIN_WINDOW_HPP