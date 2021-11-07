#ifndef PIPE_HPP
#define PIPE_HPP

#include <cstdio>
#include <cstring>
#include <string>

namespace window {

class Pipe
{
public:
    Pipe(const std::string& cmd)
    {
        _pipe = popen(cmd.c_str(), "r");
    }

    ~Pipe()
    {
        if (_pipe)
        {
            pclose(_pipe);
        }
    }

    bool GetLine(std::string& line)
    {
        bool result = false;
        bool go_on = true;
        char buffer[1024];

        line.erase();
        while(go_on && _pipe && fgets(buffer, sizeof(buffer), _pipe))
        {
            line += buffer;
            go_on = (strnlen(buffer, sizeof(buffer)) >= (sizeof(buffer)-1));
            result = true;
        }

        return result;
    }

private:
    FILE* _pipe;
};

}   // end namespace
#endif  // PIPE_HPP