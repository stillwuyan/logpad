#include "windows/mainwindow.hpp"
#include "windows/demowindow.hpp"
#include "windows/searchwindow.hpp"

using namespace window::logpad;

MainWindow::MainWindow(const std::string& name)
: WindowBase(name)
, _selected(-1)
, _open_file(nullptr)
{
    _key_handler = {
        {
            [](ImGuiIO& io) {return ImGui::IsKeyPressed(290/*F1*/);},
            [this]() {
                UpdateChildWindow("Demo", std::unique_ptr<WindowChild>(new DemoWindow()));
            }
        },
        {
            [](ImGuiIO& io) {
                return ImGui::IsKeyPressed(292/*F3*/);
            },
            [this]() {
                UpdateChildWindow("Search", std::unique_ptr<WindowChild>(new SearchWindow()));
            }
        },
        {
            [](ImGuiIO& io) {
                return io.KeyCtrl && ImGui::IsKeyPressed(79/*o*/);
            },
            [this]() {
                _open_file = std::make_unique<FileDialog>([this](const std::string& file) {
                    _reader.Open(file);
                });
            }
        }
    };

    memset(_search_text, 0, 1024);
}

void MainWindow::Show()
{
    ImGui::Begin(_name.c_str(), NULL, _flags);

    // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::InputText("Search text", _search_text, 1024);

    ImGui::BeginChild("itemlist", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar); 

    ImGuiListClipper clipper;
    clipper.Begin(_reader.LineNo());
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
        {
            auto ret = ImGui::Selectable(_reader[i].c_str(), (i==_selected) ? true : false);
            if (ret)
            {
                _selected = i;
            }
            HighlightMatch(_reader[i]);
        }
    }
    clipper.End();

    ImGui::EndChild();

    ImGui::End();

    ShowDialog();
}

void MainWindow::HighlightMatch(const std::string& text)
{
    const ImVec2 p0 = ImGui::GetItemRectMin();
    const ImGuiStyle& style = ImGui::GetStyle();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    std::smatch match;
    bool result = false;
    try
    {
        result = std::regex_search(text, match, std::regex(_search_text, std::regex_constants::icase));
    }
    catch (std::regex_error&)
    {
        return;
    }

    if (result)
    {
        const char* text_str = text.c_str();
        const auto& substr = match[0].str();
        auto pos = text.find(substr);
        if (pos != std::string::npos)
        {
            auto text_pos = ImGui::CalcTextSize(text_str, text_str + pos, true);
            ImVec2 calc_pos = ImVec2(p0.x + int(style.ItemSpacing.x * 0.5f) + text_pos.x,
                                     p0.y + int(style.ItemSpacing.y * 0.5f));
            draw_list->AddText(calc_pos, IM_COL32(200, 200, 0, 200), substr.c_str());
        }
    }
    return;
}

void MainWindow::ShowDialog()
{
    if (_open_file && !_open_file->Popup())
    {
        _open_file.reset();
    }
}