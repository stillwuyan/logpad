#include "windows/searchwindow.hpp"

using namespace window::logpad;

SearchWindow::SearchWindow(const std::string& name, const std::string& file)
: WindowChild(name)
, _file(file)
, _selected(-1)
, _searching(false)
{
    memset(_search_text, 0, 1024);
}

void SearchWindow::Show()
{
    ImGui::InputText("Search text", _search_text, 1024);
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        _searching = true;
    }

    ImGuiListClipper clipper;
    clipper.Begin(_lines.size());
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
        {
            auto ret = ImGui::Selectable(_lines[i].c_str(), (i==_selected) ? true : false);
            if (ret)
            {
                _selected = i;
            }
            // HighlightMatch(_reader[i]);
        }
    }
    clipper.End();
}