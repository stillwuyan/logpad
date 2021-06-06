#ifndef WINDOW_CONTAINER_HPP
#define WINDOW_CONTAINER_HPP

namespace window {

class WindowContainer
{
public:
    WindowContainer() = default;
    virtual ~WindowContainer() = default;

    virtual void Draw() = 0;
    virtual void Resize(int width, int height) = 0;
};

}   // namespace end

#endif  // WINDOW_CONTAINER_HPP