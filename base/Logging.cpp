#include "Logging.h"
#include "Timestamp.h"
#include <stdio.h>
#include "CurrentThread.h"
using namespace muduo;

namespace muduo
{
namespace detail
{
const char *LoggerLever[]{"TRACE", "DEBUG", "INFO", "WARN",
                          "ERROR", "FATAL", "NUM_LOG_LEVELS"};
} // namespace detail
OutPutFunction g_output;
void setOutput(OutPutFunction output)
{
  g_output = std::move(output);
}
} // namespace muduo
Logger::SourceFile::SourceFile(const char *filename)
    : data_(filename),
      size_(strlen(filename)) //
{
  const char *pos = strrchr(filename, '/');
  if (pos)
  {
    data_ = pos + 1;
    size_ -= static_cast<int>(data_ - filename);
  }
}

const char *Logger::SourceFile::data() const { return data_; };
int Logger::SourceFile::length() const { return size_; }

LogStream &operator<<(LogStream &stream, const Logger::SourceFile &file)
{
  stream.append(file.data(), file.length());
  return stream;
}
LogStream &operator<<(LogStream &stream, const Logger::LogLever &level)
{
  stream << detail::LoggerLever[level];
  return stream;
}

Logger::Iml::Iml(SourceFile file, int line, LogLever level)
    : file_(file), line_(line), loglevel_(level)
{
  Timestamp stamp = Timestamp::now();
  std::string timestamp = stamp.toFormattedString(true);
  stream_ << timestamp << "Z " << muduo::CurrentThread::tid() << " " << level << " ";
}

LogStream &Logger::stream() { return iml_.stream_; }

Logger::Logger(SourceFile file, int line, LogLever lever)
    : leverl_(lever), iml_(Iml(file, line, lever)) {}

Logger::~Logger()
{
  LogStream &stream = iml_.stream_;
  stream << " - " << iml_.file_ << ":" << iml_.line_ << "\n";
  g_output(stream.getBuffer().data(), stream.getBuffer().length());
  // printf("\n %s",stream.getBuffer().data());
}
