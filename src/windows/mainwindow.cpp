#include "windows/mainwindow.hpp"
#include "windows/demowindow.hpp"
#include "windows/searchwindow.hpp"
#include "framework/common.hpp"

using namespace window::logpad;

MainWindow::MainWindow(const std::string& name)
: WindowBase(name)
, _selected(-1)
, _open_file(nullptr)
{
    _key_handler = {
        {   
            "Demo",
            {
                [](ImGuiIO&) {
                    return ImGui::IsKeyPressed(290/*F1*/);
                },
                [this]() {
                    UpdateChildWindow("Demo", std::unique_ptr<WindowChild>(new DemoWindow("Demo")));
                }
            }
        },
        {
            "Search",
            {
                [](ImGuiIO&) {
                    return ImGui::IsKeyPressed(292/*F3*/);
                },
                [this]() {
                    UpdateChildWindow("Search", std::unique_ptr<WindowChild>(new SearchWindow("Search", _reader.CurrentFile())));
                }
            }
        },
        {
            "Open",
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
        }
    };

    memset(_search_text, 0, 1024);
}

void MainWindow::Show()
{
    ImGui::Begin(_name.c_str(), NULL, _flags);

    // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::InputText("Highlight", _search_text, 1024);

    ImGui::Text("Total lines: %lld", _reader.LineNo());

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
            utility::HighlightMatch(_reader[i], _search_text);
        }
    }
    clipper.End();

    ImGui::EndChild();

    ImGui::End();

    ShowDialog();
}

void MainWindow::ShowDialog()
{
    if (_open_file && !_open_file->Popup())
    {
        _open_file.reset();
    }
}