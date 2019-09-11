#ifndef MUDUO_BASE_ASYNCLOGGING_H
#define MUDUO_BASE_ASYNCLOGGING_H
#include "LogStream.h"
#include <vector>
#include <mutex>
#include <condition_variable>
#include<thread>
namespace muduo
{
class AsyncLogging
{
public:
    AsyncLogging(int flushInterval = 3);
    ~AsyncLogging();
    void append(const char * data, int length);
    void threadFunc();
    void start();
    void stop(); 
private:
    typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer> > BufferVector;
    typedef BufferVector::value_type BufferPtr;
    BufferPtr nextbuffer_;
    BufferPtr currentbuffer_;
    BufferVector buffers_;
    std::mutex mutex_;
    std::condition_variable cond_;
    int flushInterval_;
    std::thread thread_;
};
} // namespace muduo

#endif