#include <fstream>
#include "windows/mainwindow.hpp"

using namespace window::logpad;

MainWindow::MainWindow(const std::string& name)
: _selected(-1)
, _width(0)
, _height(0)
, _resize(false)
, _name(name)
, _show_window {
    {290, false, MainWindow::ShowDemo},
    {292, false, MainWindow::ShowSearch},
}
{
    _flags = ImGuiWindowFlags_NoCollapse
                                  | ImGuiWindowFlags_NoResize
                                  | ImGuiWindowFlags_NoScrollbar
                                  | ImGuiWindowFlags_HorizontalScrollbar
                                  | ImGuiWindowFlags_NoTitleBar
                                  | ImGuiWindowFlags_NoBringToFrontOnFocus
                                  // | ImGuiWindowFlags_NoBackground
                                  | ImGuiWindowFlags_NoMove;

    std::string log = "C:/Data/Users/xu_q2/Tmp/cns/log/CNSDEFKPM-23802/2021-03-03-14_16_17/2021-03-03-14_16_17.log";
    std::ifstream file(log);
    std::string line;
    while (std::getline(file, line))
    {
        _data.emplace_back(line);
    }
    _lines_count = _data.size();
}

void MainWindow::Draw()
{
    if (_resize)
    {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(_width, _height), ImGuiCond_Always);
        _resize = false;
    }

    ImGui::Begin(_name.c_str(), NULL, _flags);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::BeginChild("itemlist", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
 
    ImGuiListClipper clipper;
    clipper.Begin(_lines_count);
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
        {
            auto ret = ImGui::Selectable(_data[i].c_str(), (i==_selected) ? true : false);
            if (ret)
            {
                _selected = i;
            }
        }
    }
    clipper.End();

    ImGui::EndChild();
    ImGui::End();

    for (auto& item : _show_window)
    {
        if (std::get<1>(item))
        {
            std::get<1>(item) = std::get<2>(item)(this);
        }
    }

    HandleKeyboard();
}

void MainWindow::Resize(int width, int height)
{
    if (_width != width || _height != height)
    {
        _resize = true;
        _width = width;
        _height = height;
    }
}

void MainWindow::HandleKeyboard()
{
    for (auto& item : _show_window)
    {
        if (ImGui::IsKeyPressed(std::get<0>(item)))
        {
            std::get<1>(item) = !std::get<1>(item);
        }
    }
}

bool MainWindow::ShowDemo()
{
    bool open = true;
    ImGui::ShowDemoWindow(&open);
    return open;
}

bool MainWindow::ShowSearch()
{
    return false;
}