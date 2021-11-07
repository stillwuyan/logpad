#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>

#define TIMES_MS std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
#define ENTRY EntryRaiiObject obj ## __LINE__ (__FUNCTION__)

#define USE_STANDARD_IO

#ifdef USE_STANDARD_IO
#include <format>
#include <iostream>
#include <iomanip>

#define LOG_FORMAT(level, ...) \
    std::cout << "[" << TIMES_MS << "][" << std::setw(5) << level << "][" << __func__ << "] " << std::format(__VA_ARGS__) << std::endl
#define LOG_FORMAT_WITHFUNC(level, func, ...) \
    std::cout << "[" << TIMES_MS << "][" << std::setw(5) << level << "][" << func << "] " << std::format(__VA_ARGS__) << std::endl
#define LOG_DEBUG(...) \
    LOG_FORMAT("DEBUG", __VA_ARGS__)
#define LOG_INFO(...) \
    LOG_FORMAT("INFO", __VA_ARGS__)
#define LOG_WARN(...) \
    LOG_FORMAT("WARN",  __VA_ARGS__)
#define LOG_ERR(...) \
    LOG_FORMAT("ERROR",  __VA_ARGS__)
#endif

struct EntryRaiiObject {
  EntryRaiiObject(const char *f) : _f(f) { LOG_FORMAT_WITHFUNC("DEBUG", _f, "Enter"); }
  ~EntryRaiiObject() { LOG_FORMAT_WITHFUNC("DEBUG", _f, "Exit"); }
  const char *_f;
};


#endif  // LOGGER_HPP