#include "framework/config.hpp"
#include "framework/pipe.hpp"
#include "input/filefilter.hpp"

using namespace window::logpad;

FileFilter::FileFilter(const std::string& file)
: _file(file)
, _lines_no(0)
{
}

void FileFilter::Search(const std::string& keyword)
{
    _lines_no = 0;
    _lines.clear();

    std::string cmd(Config::GetInstance().filter_tools);
    cmd += " \"";
    cmd += keyword;
    cmd += "\" \"";
    cmd += _file;
    cmd += "\"";

    Pipe reader(cmd);
    std::string line;
    while(reader.GetLine(line))
    {
        _lines.emplace_back(line);
        _lines_no++;
    }
}

const std::string& FileFilter::operator[](size_t index)
{
    // std::lock_guard<std::mutex> lck(_lines_mtx);
    return _lines.at(index);

}

size_t FileFilter::LineNo()
{
    // std::lock_guard<std::mutex> lck(_lines_mtx);
    return _lines_no;
}
