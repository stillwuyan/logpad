#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <vector>
#include <string>
#include <regex>
#include <functional>
#include <imgui/imgui.h>
#include <imgui-addons/FileBrowser/ImGuiFileBrowser.h>
#include "framework/windowbase.hpp"

namespace window { namespace logpad {

class MainWindow : public WindowBase
{
public:
    MainWindow(const std::string& name);
    ~MainWindow() = default;

    void Draw() final override;
    void Resize(int width, int height) final override;

private:
    using KeyPressFunc = std::function<bool(ImGuiIO&)>;
    using WindowProc = std::function<bool(MainWindow*)>;

    void HandleKeyboard();
    void HighlightMatch(const std::string& text);
    bool ShowDemo();
    bool ShowSearch();
    bool OpenFile();

    ImGuiWindowFlags _flags;
    bool _resize;
    int _selected;
    int _width;
    int _height;
    size_t _lines_count;
    const std::string _name;
    std::vector<std::string> _data;
    std::vector<std::tuple<KeyPressFunc, bool, WindowProc>> _key_handler;
    imgui_addons::ImGuiFileBrowser _file_dialog;
    char _search_text[1024];
};

}}  // namespace end

#endif  // MAIN_WINDOW_HPP