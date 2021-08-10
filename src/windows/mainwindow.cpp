#include "windows/mainwindow.hpp"
#include "windows/demowindow.hpp"
#include "windows/searchwindow.hpp"
#include "framework/common.hpp"

using namespace window::logpad;

MainWindow::MainWindow(const std::string& name)
: WindowContainer(name)
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

    // ImGuiListClipper clipper;
    // clipper.Begin(_reader.LineNo());
    // while (clipper.Step())
    // {
    //     for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
    //     {
    //         auto ret = ImGui::Selectable(_reader[i].c_str(), (i==_selected) ? true : false);
    //         if (ret)
    //         {
    //             _selected = i;
    //         }
    //         utility::HighlightMatch(_reader[i], _search_text);
    //     }
    // }
    // clipper.End();

    ImVec2 list_begin;
    ImVec2 list_end;
    static int current_column = 0;
    static int focus_column = 0;
    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable /*| ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_NoHostExtendY*/;
    float column_width;
    static int key_state = 0;


    if (ImGui::BeginTable("", 2, flags/*, ImVec2(700.0, 0.0)*/))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("One", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Two", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow();

        char CellText[256];
        const char* WrapText[2] = {
            "Some contents Some contents Some contents Some contents Some contents Some contents Some contents",
            "Some contents Some contents"
        };
        ImVec2 text_size;

        column_width = ImGui::GetColumnWidth();
        list_begin = ImGui::GetCursorScreenPos();
        list_end = ImGui::GetContentRegionAvail();

        for (int row = 0; row < 100; row++)
        {
            int real_row = row + current_column;
            text_size = ImGui::CalcTextSize(WrapText[row%2], NULL, false, column_width);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            sprintf(CellText, "Row %02d", real_row);
            bool is_selected = false;
            if (focus_column == real_row)
            {
                is_selected = true;
            }
            ImGui::Selectable(CellText, is_selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_Disabled, ImVec2(0.0, text_size.y));
            ImGui::TableNextColumn();
            ImGui::TextWrapped(WrapText[real_row%2]);

            if (focus_column == real_row)
            {
                auto pos = ImGui::GetCursorScreenPos();
                ImGui::GetForegroundDrawList()->AddRectFilled(pos, ImVec2(pos.x + 100.0, pos.y + 5.0), IM_COL32( 255, 0, 0, 255 ) );

                ImVec2 vMin = ImGui::GetWindowPos();
                ImVec2 vMax = ImGui::GetWindowSize();

                vMax.x += ImGui::GetWindowPos().x;
                vMax.y += ImGui::GetWindowPos().y;

                ImGui::GetForegroundDrawList()->AddRect( vMin, vMax, IM_COL32( 255, 255, 0, 255 ) );

                if (key_state == 1 && pos.y >= vMax.y)
                {
                    ImGui::SetScrollY(ImGui::GetScrollY() + 200.0f);
                    key_state = 0;
                }
                else if (key_state == 2 && pos.y - 10 <= vMin.y)
                {
                    ImGui::SetScrollY(ImGui::GetScrollY() - 200.0f);
                    key_state = 0;
                }
            }
        }
        if (ImGui::GetScrollY()/ImGui::GetScrollMaxY() >= 1.0)
        {
            current_column += 50;
            ImGui::SetScrollY(ImGui::GetScrollMaxY()/2);
        }
        else if (current_column >= 50 && ImGui::GetScrollY() == 0.0)
        {
            current_column -= 50;
            ImGui::SetScrollY(ImGui::GetScrollMaxY()/2);
        }

        if (ImGui::IsKeyPressed(74/*J*/) && focus_column < 300)
        {
            focus_column += 1;
            key_state = 1;
        }
        else if (ImGui::IsKeyPressed(75/*K*/) && focus_column >= 0)
        {
            focus_column -= 1;
            key_state = 2;
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