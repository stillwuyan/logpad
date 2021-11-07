#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <imgui.h>
#include "windows/base/windowcontainer.hpp"
#include "sources/filereader.hpp"
#include "windows/filedialog.hpp"

namespace window { namespace logpad {

class MainWindow : public WindowContainer
{
public:
    MainWindow(const std::string& name);
    ~MainWindow() = default;

    void Show() final override;

private:
    void ShowDialog();

    int _selected;
    FileReader _reader;
    char _search_text[1024];
    std::unique_ptr<FileDialog> _open_file;
    ImGuiListClipper _clipper;
};

}}  // namespace end

#endif  // MAIN_WINDOW_HPP