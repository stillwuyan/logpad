#include "windows/mainwindow.hpp"

using namespace window::logpad;

MainWindow::MainWindow(const std::string& name)
: _resize(false)
, _selected(-1)
, _width(0)
, _height(0)
, _name(name)
, _key_handler {
    {[](ImGuiIO& io) {return ImGui::IsKeyPressed(290/*f1*/);}, false, MainWindow::ShowDemo},
    {[](ImGuiIO& io) {return ImGui::IsKeyPressed(292/*f3*/);}, false, MainWindow::ShowSearch},
    {[](ImGuiIO& io) {return io.KeyCtrl && ImGui::IsKeyPressed(79/*o*/);}, false, MainWindow::OpenFile},
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
    memset(_search_text, 0, 1024);
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

    for (auto& item : _key_handler)
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
    auto& io = ImGui::GetIO();
    for (auto& item : _key_handler)
    {
        if (std::get<0>(item)(io))
        {
            std::get<1>(item) = !std::get<1>(item);
        }
    }
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

bool MainWindow::ShowDemo()
{
    bool open = true;
    ImGui::ShowDemoWindow(&open);
    return open;
}

bool MainWindow::ShowSearch()
{
    ImGui::SetNextWindowSize(ImVec2(600.0f, 550.0f), ImGuiCond_Once);
    ImGui::Begin("Test");

    static float wrap_width = 200.0f;
    ImGui::SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (int n = 0; n < 2; n++)
    {
        ImGui::Text("[%d]Test paragraph screenf", n);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 marker_min = ImVec2(pos.x + wrap_width, pos.y);
        ImVec2 marker_max = ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight());
        ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
        if (n == 0)
        {
            ImGui::Text("The lazy dog is a good dog. This paragraph should fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
        }
        else
        {
            ImGui::Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");
        }

        // Draw actual text bounding box, following by marker of our expected limit (should not overlap!)
        draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 255));
        draw_list->AddRectFilled(marker_min, marker_max, IM_COL32(255, 0, 255, 255));
        ImGui::PopTextWrapPos();
    }

    ImVec2 size(200.0f, 100.0f);
    ImVec2 offset(10.0f, 10.0f);
    for (int n = 0; n < 3; n++)
    {
        if (n > 0)
        {
            ImGui::SameLine();
        }
        ImGui::PushID(n);
        ImGui::BeginGroup();

        ImGui::InvisibleButton("##empty", size);
        const ImVec2 p0 = ImGui::GetItemRectMin();
        const ImVec2 p1 = ImGui::GetItemRectMax();
        const std::string text_str("Line 1 hello hehehehehehehehehehehehehehehe\nLine 2 clip me!\nLine 2 clip me!\nLine 2 clip me!\nLine 2 clip me!"); 
        const ImVec2 text_pos = ImVec2(p0.x + offset.x, p0.y + offset.y);

        switch (n)
        {
        case 0:
            ImGui::PushClipRect(p0, p1, true);
            draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
            draw_list->AddText(text_pos, IM_COL32_WHITE, text_str.c_str());
            ImGui::PopClipRect();
            break;
        case 1:
            draw_list->PushClipRect(p0, p1, true);
            draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
            draw_list->AddText(text_pos, IM_COL32_WHITE, text_str.c_str());
            {
                std::string substr("hehe");
                auto pos = text_str.find(substr);
                if (pos != std::string::npos)
                {
                    auto text_width = ImGui::CalcTextSize(text_str.c_str(), text_str.c_str()+pos, true);
                    ImVec2 text_pos_new = ImVec2(text_pos.x + text_width.x, text_pos.y);
                    draw_list->AddText(text_pos_new, IM_COL32(255,255,0,255), "hehe");
                }
            }
            
            draw_list->PopClipRect();
            break;
        case 2:
            ImVec4 clip_rect(p0.x, p0.y, p1.x, p1.y); // AddText() takes a ImVec4* here so let's convert.
            draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
            draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), text_pos, IM_COL32_WHITE, text_str.c_str(), NULL, 0.0f, &clip_rect);
            break;
        }
        ImGui::EndGroup();
        ImGui::PopID();
    }

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));
    ImGui::Text("This is error text");
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.2f, 1.0f));
    ImGui::Text("This is another text");
    ImGui::PopStyleColor();

    ImGui::End();
    return true;
}

bool MainWindow::OpenFile()
{
    if (!ImGui::IsPopupOpen("Open File"))
    {
        ImGui::OpenPopup("Open File");
    }

    if (_file_dialog.showFileDialog("Open File", 
                                    imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,
                                    ImVec2(800, 600),
                                    ".log,old,.bin"))
    {
        // read file
        _reader.Open(_file_dialog.selected_path);
    }

    return ImGui::IsPopupOpen("Open File");
}