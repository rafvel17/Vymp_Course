#ifndef __LOGGING_HPP__
#define __LOGGING_HPP__

/*
Test this with something like:
g++ -std=c++11 -x c++ -pthread -DLOGGING_LEVEL_ALL -DTEST_LOGGING logging.hpp
-o logging_test
./logging_test
*/

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

//	Friggin' Microsoft and their global generic macros.
#if defined(_WIN32)
#include "color.hpp" //	Pull in header from https://github.com/imfl/color-console
#if defined(ERROR)
#undef ERROR //  Unfortunately ERROR is defined as a macro in windgi.h...
#endif
#if defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif
#endif

namespace logging
{

// TODO: use macros (again) so __FILE__ __LINE__ could be automatically added
// to certain error levels?

// the log levels we support
enum class LogLevel : uint8_t
{
  TRACE = 0,
  DEBUG = 1,
  INFO = 2,
  WARN = 3,
  ERROR = 4
};
struct EnumHasher
{
  template <typename T>
  std::size_t
  operator() (T t) const
  {
    return static_cast<std::size_t> (t);
  }
};
const std::unordered_map<LogLevel, std::string, EnumHasher> uncolored{
  { LogLevel::ERROR, " [ERROR] " },
  { LogLevel::WARN, " [WARN] " },
  { LogLevel::INFO, " [INFO] " },
  { LogLevel::DEBUG, " [DEBUG] " },
  { LogLevel::TRACE, " [TRACE] " }
};

// all, something in between, none or default to info
#if defined(LOGGING_LEVEL_ALL) || defined(LOGGING_LEVEL_TRACE)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::TRACE;
#elif defined(LOGGING_LEVEL_DEBUG)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::DEBUG;
#elif defined(LOGGING_LEVEL_WARN)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::WARN;
#elif defined(LOGGING_LEVEL_ERROR)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::ERROR;
#elif defined(LOGGING_LEVEL_NONE)
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::ERROR + 1;
#else
constexpr LogLevel LOG_LEVEL_CUTOFF = LogLevel::INFO;
#endif

#if defined(_WIN32)
struct tm *
gmtime_r (const time_t *timer, struct tm *buf)
{
#if defined(_MSC_VER)
  gmtime_s (buf, timer);
#elif defined(__BORLANDC__)
  gmtime_s (timer, buf);
#endif
  return buf;
}

#endif

// returns formated to: 'year/mo/dy hr:mn:sc.xxxxxx'
inline std::string Timestamp ();

// logger base class, not pure virtual so you can use as a null logger if you
// want
using logging_config_t = std::unordered_map<std::string, std::string>;
class Logger
{
public:
  static void SetConfig (const logging_config_t &config);
  static Logger *GetInstance (const std::string &id);
  static void destroy ();
  void Log (const std::string &message, const LogLevel level);
  void Log (const std::string &message);

protected:
  virtual ~Logger (){};

private:
  Logger (const std::string &id);
  void Reopen ();

private:
  std::string m_id;
  inline static std::unordered_map<std::string, Logger *> s_instances;
  inline static logging_config_t s_config;
  inline static std::mutex s_lock;
  inline static std::string s_dirName;
  std::mutex m_lock;
  std::string m_fileName;
  std::ofstream m_file;
  std::chrono::seconds m_reopenInterval;
  std::chrono::system_clock::time_point m_lastReopen;
};
// statically log manually without the macros below
inline void
log (const std::string &id, const std::string &message, const LogLevel level)
{
  Logger::GetInstance (id)->Log (message, level);
}
// statically log manually without a level or maybe with a custom one
inline void
log (const std::string &id, const std::string &message)
{
  Logger::GetInstance (id)->Log (message);
}
// these standout when reading code
inline void
TRACE (const std::string &id, const std::string &message)
{
  Logger::GetInstance (id)->Log (message, LogLevel::TRACE);
}
inline void
DEBUG (const std::string &id, const std::string &message)
{
  Logger::GetInstance (id)->Log (message, LogLevel::DEBUG);
}
inline void
INFO (const std::string &id, const std::string &message)
{
  Logger::GetInstance (id)->Log (message, LogLevel::INFO);
}
inline void
WARN (const std::string &id, const std::string &message)
{
  Logger::GetInstance (id)->Log (message, LogLevel::WARN);
}
inline void
ERROR (const std::string &id, const std::string &message)
{
  Logger::GetInstance (id)->Log (message, LogLevel::ERROR);
}

} //	end namespace logging

#endif //__LOGGING_HPP__