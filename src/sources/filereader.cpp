#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include "sources/filereader.hpp"

using namespace window::logpad;

FileReader::FileReader(const std::string& file)
: _file(file)
, _lines_no(0)
, _loading(false)
{
    Prefetch();
}

FileReader::FileReader()
: _lines_no(0)
{}

void FileReader::Open(const std::string& file)
{
    if (_loading.load(std::memory_order_relaxed))
    {
        _loading.store(false, std::memory_order_release);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    _file = file;
    if (_lines_no > 0)
    {
        _lines_no = 0;
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
    std::lock_guard<std::mutex> lck(_lines_mtx);
    return _lines_no;
}


const std::string& FileReader::CurrentFile()
{
    return _file;
}

void FileReader::Prefetch()
{
    std::thread t([this]{
        _loading.store(true, std::memory_order_release);
        std::ifstream reader(_file);
        std::string line;
        while (_loading.load(std::memory_order_relaxed) && std::getline(reader, line))
        {
            std::lock_guard<std::mutex> lck(_lines_mtx);
            _lines.emplace_back(line);
            _lines_no += 1;
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
