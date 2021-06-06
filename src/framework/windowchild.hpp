#ifndef WINDOW_CHILD_HPP
#define WINDOW_CHILD_HPP

class WindowChild
{
public:
    WindowChild() = default;
    virtual ~WindowChild() = default;

    virtual bool Show() = 0;

};

#endif  // WINDOW_CHILD_HPP