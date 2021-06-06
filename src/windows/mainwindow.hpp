#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <vector>
#include <string>
#include <regex>
#include <functional>
#include <imgui/imgui.h>
#include "framework/windowbase.hpp"
#include "input/filereader.hpp"
#include "windows/filedialog.hpp"

namespace window { namespace logpad {

class MainWindow : public WindowBase
{
public:
    MainWindow(const std::string& name);
    ~MainWindow() = default;

    void Show() final override;

private:
    void HighlightMatch(const std::string& text);
    void ShowDialog();

    int _selected;
    FileReader _reader;
    char _search_text[1024];
    std::unique_ptr<FileDialog> _open_file;
};

}}  // namespace end

#endif  // MAIN_WINDOW_HPP