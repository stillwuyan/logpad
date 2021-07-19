#ifndef FILE_FILTER_HPP
#define FILE_FILTER_HPP

#include <string>

namespace windows { namespace logpad {

class FileFilter
{
public:
    FileFilter(const string& file);
    ~FileFilter() = default;

private:
    void Prefetch();

    const std::string _file;
};

}}  // end namespace
#endif  // FILE_FILTER_HPP