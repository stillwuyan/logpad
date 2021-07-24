#ifndef FILE_FILTER_HPP
#define FILE_FILTER_HPP

#include <string>
#include <vector>

namespace window { namespace logpad {

class FileFilter
{
public:
    FileFilter(const std::string& file);
    ~FileFilter() = default;

    void Search(const std::string& keyword);
    const std::string& operator[](size_t index);
    size_t LineNo();

private:
    const std::string _file;
    size_t _lines_no;
    std::vector<std::string> _lines;
};

}}  // end namespace
#endif  // FILE_FILTER_HPP