#include <cstdlib>
#include "framework/config.hpp"
#include "input/filefilter.hpp"

using namespace window::logpad;

FileFilter::FileReader(const std::string& file)
: _file(file)
{
    Prefetch();
}

void FileFilter::Prefetch()
{
    auto cmd = std::format("{} {} {}", Config::GetInstance().filter_tools, "123", _file);
    std::system(cmd)
}