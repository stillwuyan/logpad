#ifndef COMMON_HPP
#define COMMON_HPP

#include <regex>
#include <imgui.h>

namespace window {

enum class WindowMode : uint8_t
{
    window = 0,
    fullscreen = 1,
    to_window = 2,
    to_fullscreen = 3,
};

namespace utility {
    static inline void HighlightMatch(const std::string& line, const std::string& search)
    {
        const ImVec2 p0 = ImGui::GetItemRectMin();
        const ImGuiStyle& style = ImGui::GetStyle();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        std::smatch match;
        bool result = false;
        try
        {
            result = std::regex_search(line, match, std::regex(search, std::regex_constants::icase));
        }
        catch (std::regex_error&)
        {
            return;
        }

        if (result)
        {
            const char* text_str = line.c_str();
            const auto& substr = match[0].str();
            auto pos = line.find(substr);
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
}

} // namespace end

#endif  // COMMON_HPP