#include "windows/mainwindow.hpp"
#include "windows/demowindow.hpp"
#include "windows/searchwindow.hpp"
#include "framework/common.hpp"

using namespace window::logpad;

MainWindow::MainWindow(const std::string& name)
: WindowContainer(name)
, _selected(0)
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
    ImGui::Text("Highlight:");
    ImGui::SameLine();
    ImGui::InputText("##Highlight", _search_text, 1024);
    ImGui::SameLine();
    ImGui::Text("  Loading: %lld", _reader.LineNo());

    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_SizingFixedFit;
    if (_reader.LineNo() > 0 && ImGui::BeginTable("", 2, flags))
    {
        double selected_item_ypos = 0.0f;
        ImGuiListClipper clipper;
        clipper.Begin(_reader.LineNo());
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                auto ret = ImGui::Selectable(std::to_string(i+1).c_str(),
                                             (i==_selected) ? true : false,
                                             ImGuiSelectableFlags_SpanAllColumns);
                _selected = (ret ? i : _selected);
                ImGui::TableNextColumn();
                ImGui::Text(_reader[i].c_str());
                utility::HighlightMatch(_reader[i], _search_text);

                selected_item_ypos = (i==_selected ? ImGui::GetCursorScreenPos().y : selected_item_ypos);
            }
        }
        clipper.End();

        if (ImGui::IsKeyPressed(74/*J*/))
        {
            if (static_cast<size_t>(_selected) < _reader.LineNo()-1)
                _selected++;
            if (_selected >= clipper.DisplayEnd-1)
                ImGui::SetScrollY(ImGui::GetScrollY() + clipper.ItemsHeight);
        }
        else if (ImGui::IsKeyPressed(75/*K*/))
        {
            if (_selected > 0)
                _selected--;
            if (_selected <= clipper.DisplayStart)
                ImGui::SetScrollY(ImGui::GetScrollY() - clipper.ItemsHeight);
        }
        ImGui::EndTable();
    }

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