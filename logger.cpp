#include "logger.h"
#include <filesystem>
#include <utility>
#include <cassert>
namespace logging
{
std::string
Timestamp ()
{
  // get the time
  std::chrono::system_clock::time_point tp = std::chrono::system_clock::now ();
  std::time_t tt = std::chrono::system_clock::to_time_t (tp);
  std::tm gmt{};
  gmtime_r (&tt, &gmt);
  std::chrono::duration<double> fractional_seconds
      = (tp - std::chrono::system_clock::from_time_t (tt))
        + std::chrono::seconds (gmt.tm_sec);
  // format the string
  std::string buffer ("year.mo.dy hr:mn");
  sprintf (&buffer.front (), "%04d.%02d.%02d %02d:%02d",
           gmt.tm_year + 1900, gmt.tm_mon + 1, gmt.tm_mday, gmt.tm_hour,
           gmt.tm_min);
  return buffer;
}
void
Logger::SetConfig (const logging_config_t &config)
{
  s_config = config;
}
void 
Logger::destroy()
{
  for(auto& instance: s_instances)
  {
    delete instance.second;
  }
}
Logger*
Logger::GetInstance (const std::string& id)
{
  if (s_instances.empty ())
    {
      s_lock.lock ();
      s_dirName = Timestamp ();
      assert(std::filesystem::create_directory (s_dirName));
      s_lock.unlock ();
    }
  if (s_instances.find (id) == s_instances.end ())
    {
      s_lock.lock ();
      if (s_instances.find (id) == s_instances.end ())
        {
          Logger* logger = new Logger(id);
          s_instances[id] = logger;
        }
      s_lock.unlock ();
    }
  return s_instances.at (id);
}
Logger::Logger (const std::string& id)
{
  // grab the file name
  m_id = id;
  m_fileName = s_dirName + "/" + id + ".txt";
  // if we specify an interval
  m_reopenInterval = std::chrono::seconds (300);
  auto interval = s_config.find ("reopen_interval");
  if (interval != s_config.end ())
    {
      try
        {
          m_reopenInterval
              = std::chrono::seconds (std::stoul (interval->second));
        }
      catch (...)
        {
          throw std::runtime_error (interval->second
                                    + " is not a valid reopen interval");
        }
    }
  Reopen ();
}
void
Logger::Log (const std::string &message, const LogLevel level)
{
  if (level < LOG_LEVEL_CUTOFF)
    {
      return;
    }
  std::string output;
  output.reserve (message.length () + 64);
  output.append (uncolored.find (level)->second);
  output.append (message);
  output.push_back ('\n');
  Log (output);
}
void
Logger::Log (const std::string &message)
{
  m_lock.lock ();
  m_file << message;
  m_file.flush ();
  m_lock.unlock ();
  Reopen ();
}
void
Logger::Reopen ()
{
  // check if it should be closed and reopened
  auto now = std::chrono::system_clock::now ();
  m_lock.lock ();
  if (now - m_lastReopen > m_reopenInterval)
    {
      m_lastReopen = now;
      try
        {
          m_file.close ();
        }
      catch (...)
        {
        }
      try
        {
          m_file.open (m_fileName, std::ofstream::out | std::ofstream::app);
          m_lastReopen = std::chrono::system_clock::now ();
        }
      catch (std::exception &e)
        {
          try
            {
              m_file.close ();
            }
          catch (...)
            {
            }
          throw e;
        }
    }
  m_lock.unlock ();
}
}
