#ifndef MUDUO_BASE_LOGSTREAM_H
#define MUDUO_BASE_LOGSTREAM_H

#include <string.h>
#include <boost/noncopyable.hpp>
#include <iostream>
#include "boost/function.hpp"
namespace muduo
{
namespace detail
{
template <int SIZE>
class FixedBuffer
{ //内部类  简单的话放在类中  复杂的放在子namespace
public:
  boost::function<void(void)> CookieCallBack;
  FixedBuffer() : cur_(data_){}; //构造析构一般不inline
  ~FixedBuffer(){};
  void append(const char *msg, size_t length)
  { // size_t
    if (length <= static_cast<size_t>(avail()))
    {
      memcpy(cur_, msg, length);
      add(length);
    }
  }
  const char *data() const
  {
    return data_;
  } // C++中const 引用的是对象时只能访问该对象的const
  // 函数，因为其他函数有可能会修改该对象的成员，编译器为了避免该类事情发生，会认为调用非const函数是错误的。
  size_t length() const { return static_cast<int>(cur_ - data_); }

  char *current() { return cur_; }
  int avail() { return static_cast<int>(end() - cur_); }
  void add(int len) { cur_ += len; }
  bool reset() { cur_ = data_; }
  void bzero() { memZero(data_, sizeof data_); }

  std::string toString() { return std::string(data_, length()); }
  //   void cookieStart();
  //   void cookierEnd();
  //   static void setCookie(CookieCallBack cookie) { cookie_ = cookie; }

private:
  char *end() { return (&data_[SIZE]); }
  // void add(size_t length) { cur_ += length; }
  char data_[SIZE];
  char *cur_;
  //   static CookieCallBack cookie_;
};
//使用静态成员函数或命名空间内的非成员函数, 尽量不要用裸的全局函数. 将一系列函数直接置于命名空间中，不要用类的静态方法模拟出命名空间的效果，类的静态方法应当和类的实例或静态数据紧密相关.
const int kSmallBuffer = 4000; //封装数据
const int kLargeBuffer = 4000 * 1000;
const int kMaxNumericSize = 32;
} // namespace detail
class LogStream
{
public:
  typedef LogStream self;
  typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;
  self &operator<<(bool v)
  {
    v ? append("1", 1) : append("0", 1);
    return *this;
  };
  self &operator<<(short);
  self &operator<<(unsigned short);
  self &operator<<(int);
  self &operator<<(unsigned int);
  self &operator<<(long);
  self &operator<<(unsigned long);
  self &operator<<(long long);
  self &operator<<(unsigned long long);

  self &operator<<(const void *);

  self &operator<<(float v);
  self &operator<<(double);
  // self& operator<<(long double);

  self &operator<<(char v)
  {                //简单函数直接在类的声明中实现即头文件中
    append(&v, 1); //若内部类 则因为隐藏性  放在cpp中实现
    return *this;  //类中第一个参数默认
  };

  self &operator<<(const char *str)
  { //传入指针要考虑为空的情况
    append(str, strlen(str));
    return *this;
  };

  self &operator<<(const unsigned char *str);

  self &operator<<(const std::string &v)
  {
    append(v.c_str(), v.length()); //能直接实现 不要调用其他函数
    return *this;
  };
  self &operator<<(const Buffer &v)
  {
    append(v.data(), v.length());
    return *this;
  };

  const Buffer &getBuffer() { return buffer_; }

  void append(const char *msg, const size_t length)
  {
    buffer_.append(msg, length);
  }

private:
  /* data */
  template <typename T>
  void formatInteger(const T val);
  Buffer buffer_;
};

} // namespace muduo

#endif