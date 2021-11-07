#include "windows/mainwindow.hpp"
#include "windows/demowindow.hpp"
#include "windows/searchwindow.hpp"
#include "common/utils.hpp"

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
                    return ImGui::IsKeyPressed(ImGuiKey_F1, false);
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
                    return ImGui::IsKeyPressed(ImGuiKey_F3, false);
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
                    return io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O, false);
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
    if (_reader.LineNo() > 0 && ImGui::BeginTable("item list", 2, flags))
    {
        double selected_item_ypos = 0.0f;
        _clipper.Begin(_reader.LineNo());
        while (_clipper.Step())
        {
            for (int i = _clipper.DisplayStart; i < _clipper.DisplayEnd; ++i)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                auto ret = ImGui::Selectable(std::to_string(i+1).c_str(),
                                             (i==_selected) ? true : false,
                                             ImGuiSelectableFlags_SpanAllColumns);
                _selected = (ret ? i : _selected);
                ImGui::TableNextColumn();
                auto match = utility::HighlightMatch(_reader[i], _search_text);
                if (match.empty()) {
                    ImGui::TextUnformatted(_reader[i].c_str());
                }
                else
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
                    ImGui::TextUnformatted(match.prefix().str().c_str());
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f,1.0f));
                    ImGui::TextUnformatted(match[0].str().c_str());
                    ImGui::PopStyleColor();
                    ImGui::SameLine();
                    ImGui::TextUnformatted(match.suffix().str().c_str());
                    ImGui::PopStyleVar();
                }

                selected_item_ypos = (i==_selected ? ImGui::GetCursorScreenPos().y : selected_item_ypos);
            }
        }
        _clipper.End();

        if (ImGui::IsKeyPressed(ImGuiKey_J, false))
        {
            if (static_cast<size_t>(_selected) < _reader.LineNo()-1)
                _selected++;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_K, false))
        {
            if (_selected > 0)
                _selected--;
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