#include <string.h>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include "LogStream.h"
#include "Logging.h"
#include "AsyncLogging.h"

void test1()
{
  while (1)
  {
    LOG_WARN << "makj1111";
  }
}

void test2()
{
  while (1)
  {
    LOG_WARN << "makj2222";
  }
}
void test3()
{
  while (1)
  {

    LOG_WARN << "makj3333";
  }
}
void test4()
{
  while (1)
  {

    LOG_WARN << "makj4444";
  }
}
void test5()
{
  while (1)
  {

    LOG_WARN << "makj5555";
  }
}
int main()
{

  muduo::AsyncLogging  asynclog(3);
  muduo::setOutput(std::bind(&muduo::AsyncLogging::append,&asynclog,std::placeholders::_1,std::placeholders::_2));

  std::thread t1(test1);
  std::thread t2(test2);
  std::thread t3(test3);
  std::thread t4(test4);
  std::thread t5(test5);
  asynclog.start();

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
 
  return 0;
}