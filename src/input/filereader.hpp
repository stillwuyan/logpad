#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <string>
#include <vector>
#include <atomic>
#include <mutex>

namespace window { namespace logpad {

class FileReader
{
public:
    FileReader(const std::string& file);
    FileReader();
    ~FileReader() = default;

    void Open(const std::string& file);
    const std::string& operator[](size_t index);
    size_t LineNo();
    const std::string& CurrentFile();

private:
    void Prefetch();
    size_t CountLine();

    std::string _file;
    std::vector<std::string> _lines;
    size_t _lines_no;
    std::atomic<bool> _loading;
    std::mutex _lines_mtx;
};

}}  // namespace end

#endif // FILE_READER_HPP