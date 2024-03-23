#ifndef UTILS_HPP
#define UTILS_HPP

#include <regex>
#include <vector>
#include <string>
#include "common/logger.hpp"

namespace window {

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