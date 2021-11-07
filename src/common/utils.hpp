#ifndef UTILS_HPP
#define UTILS_HPP

#include <regex>
#include <vector>
#include <string>
#include "common/logger.hpp"

namespace window {

enum class WindowMode : uint8_t
{
    window = 0,
    fullscreen = 1,
    to_window = 2,
    to_fullscreen = 3,
};

namespace utility {
    static inline std::smatch HighlightMatch(const std::string& line, const std::string& search)
    {
        std::smatch match;
        if (search.empty())
        {
            return match;
        }

        try
        {
            std::regex_search(line, match, std::regex(search, std::regex_constants::icase));
        }
        catch (std::regex_error&) {}

        return match;
    }
}

} // namespace end

#endif  // UTILS_HPP