#ifndef MUDUO_BASE_LOGGING_H
#define MUDUO_BASE_LOGGING_H

#include <string.h>
#include <boost/noncopyable.hpp>
#include <iostream>
#include "LogStream.h"
#include <functional>
namespace muduo
{

class Logger : boost::noncopyable
{
public:
  enum LogLever
  {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  class SourceFile
  {
  public:
    template <int SIZE>
    SourceFile(const char (&arr)[SIZE]) : data_(arr), size_(SIZE - 1)
    {
      const char *pos = strrchr(arr, '/');
      if (pos)
      {
        data_ = pos + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char *filename);
    const char *data() const;
    int length() const;

  private:
    const char *data_; //所指对象的内容不变
    int size_;
  };

  Logger(SourceFile file, int line, LogLever lever);
  ~Logger();
  void setLoggerLever(LogLever lever) { leverl_ = lever; }
  LogLever getLoggerLever() { return leverl_; }
  LogStream &stream();
  // LogStream& stream(){
  //   return iml_
  // }

private:
  class Iml
  {
  public:
    Iml(SourceFile file, int line, LogLever lever);
    ~Iml() = default;
    SourceFile file_;
    int line_;
    LogLever loglevel_;
    LogStream stream_;
  };
  LogLever leverl_;
  Iml iml_;
};
typedef std::function<void(const char *, int)> OutPutFunction;
void setOutput(OutPutFunction func);
// LogStream &operator<<(LogStream &, Logger::SourceFile &);
// LogStream &operator<<(LogStream &, Logger::LogLever &);
#define LOG_WARN muduo::Logger(__FILE__, __LINE__, muduo::Logger::WARN).stream()
#define LOG_ERROR \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::ERROR).stream()
#define LOG_FATAL \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::FATAL).stream()
#define LOG_SYSERR muduo::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL muduo::Logger(__FILE__, __LINE__, true).stream()
} // namespace muduo

#endif // MUDUO_BASE_LOGGING_H