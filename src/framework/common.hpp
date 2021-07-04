#ifndef COMMON_HPP
#define COMMON_HPP

namespace window {

enum class WindowMode : uint8_t
{
    window = 0,
    fullscreen = 1,
    to_window = 2,
    to_fullscreen = 3,
};

} // namespace end

#endif  // COMMON_HPP