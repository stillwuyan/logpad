#ifndef WINDOWN_BASE_HPP
#define WINDOWN_BASE_HPP

namespace window {

class WindowBase
{
public:
    WindowBase() = default;
    virtual ~WindowBase() = default;

    virtual void Draw() = 0;
    virtual void Resize(int width, int height) = 0;
};

}   // namespace end

#endif  // WINDOW_BASE_HPP