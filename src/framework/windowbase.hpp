#ifndef WINDOW_BASE_HPP
#define WINDOW_BASE_HPP

namespace window {

class WindowBase
{
public:
    WindowBase() = default;
    virtual ~WindowBase() = default;

    virtual bool Draw() = 0;
    virtual void Resize(int width, int height) = 0;
};

}   // namespace end

#endif  // WINDOW_BASE_HPP