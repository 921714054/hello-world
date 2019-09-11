#include "LogStream.h"
#include <stdio.h>
using namespace muduo; //只在cpp文件里面using namespace 别名
namespace muduo
{
namespace detail
{
}
template <typename T>
int convert(char *current, T val)
{
  bool flag = false;
  if (val == 0)
  {
    *current = '0';
    current++;
    return 1;
  }
  else if (val < 0)
  {
    flag = true;
    val = -val;
  }
  unsigned char lsd = 0;
  char *p = current;
  while (val > 0)
  {
    lsd = val % 10;
    *current = '0' + lsd;
    val = val / 10;
    current++;
  }
  if (flag)
  {
    *current = '-';
    current++;
  }
  *current = '\0';
  std::reverse(p, current);
  return static_cast<int> (current - p);
}
} // namespace muduo

template <typename T> //模板实现放在头文件里面  注意放在类里面默认是inline
void LogStream::formatInteger(T val)
{
  if (buffer_.avail() > detail::kMaxNumericSize)
  {
    int len = convert(buffer_.current(), val);
    buffer_.add(len);
  }
}

typedef LogStream self;
self &LogStream::operator<<(short v)
{
  formatInteger(v);
  return *this;
}
self &LogStream::operator<<(unsigned short v)
{
  formatInteger(v);
  return *this;
}
self &LogStream::operator<<(int v)
{
  formatInteger(v);
  return *this;
}
self &LogStream::operator<<(unsigned int v)
{
  formatInteger(v);
  return *this;
}
self &LogStream::operator<<(long v)
{
  formatInteger(v);
  return *this;
}
self &LogStream::operator<<(unsigned long v)
{
  formatInteger(v);
  return *this;
}
self &LogStream::operator<<(long long v)
{
  formatInteger(v);
  return *this;
}
self &LogStream::operator<<(unsigned long long v)
{
  formatInteger(v);
  return *this;
}
self &LogStream::operator<<(const void *) {}
self &LogStream::operator<<(float v)
{
  return (*this) << (static_cast<double>(v));
}
self &LogStream::operator<<(double v)
{
  if (buffer_.avail() >= detail::kMaxNumericSize)
  {
    int len = snprintf(buffer_.current(), detail::kMaxNumericSize, "%.12g", v);
    buffer_.add(len);
  }
  return *this;
}
