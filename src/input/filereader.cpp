#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include "input/filereader.hpp"

using namespace window::logpad;

FileReader::FileReader(const std::string& file)
: _file(file)
, _lines_no(0)
{
    Prefetch();
}

FileReader::FileReader()
: _lines_no(0)
{}

void FileReader::Open(const std::string& file)
{
    _file = file;
    if (_lines_no > 0)
    {
        _lines_no.store(0, std::memory_order_release);
        _lines.clear();
    }
    Prefetch();
}

const std::string& FileReader::operator[](size_t index)
{
    std::lock_guard<std::mutex> lck(_lines_mtx);
    return _lines.at(index);

}

size_t FileReader::LineNo()
{
    return _lines_no.load(std::memory_order_acquire);
}

void FileReader::Prefetch()
{
    std::thread t([this]{
        std::ifstream reader(_file);
        std::string line;
        while (std::getline(reader, line))
        {
            std::lock_guard<std::mutex> lck(_lines_mtx);
            _lines.emplace_back(line);
            _lines_no.fetch_add(1, std::memory_order_release);
        }
    });
    t.detach();
}

size_t FileReader::CountLine()
{
    std::ifstream reader(_file);
    size_t count = 0;
    std::vector<char> buffer(1024*1024);
    while (reader)
    {
        reader.read(&buffer[0], buffer.size());
        if (reader.gcount() < static_cast<std::streamsize>(buffer.size()))
        {
            buffer.resize(reader.gcount());
        }
        count += std::count(buffer.begin(), buffer.end(), '\n');
    }
    return count;
}
