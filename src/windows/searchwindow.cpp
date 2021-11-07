#include "windows/searchwindow.hpp"
#include "common/utils.hpp"

using namespace window::logpad;

SearchWindow::SearchWindow(const std::string& name, const std::string& file)
: WindowChild(name)
, _filter(file)
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
        _filter.Search(_search_text);
    }

    ImGuiListClipper clipper;
    clipper.Begin(_filter.LineNo());
    while (clipper.Step())
    {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
        {
            auto ret = ImGui::Selectable(_filter[i].c_str(), (i==_selected) ? true : false);
            if (ret)
            {
                _selected = i;
            }
            utility::HighlightMatch(_filter[i], _search_text);
        }
    }
    clipper.End();
}