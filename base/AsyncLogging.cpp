#include "AsyncLogging.h"
// #include "LogFile.h"
#include "Timestamp.h"
using namespace muduo;
namespace muduo
{
namespace detail
{
std::mutex mutex;
std::condition_variable cond;
} // namespace detail
} // namespace muduo

AsyncLogging::AsyncLogging(int flushInterval) : nextbuffer_(new Buffer()),
                                                currentbuffer_(new Buffer()),
                                                flushInterval_(flushInterval)
{
}

AsyncLogging::~AsyncLogging()
{
    stop();
}
void AsyncLogging::append(const char *data, int length)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (length < currentbuffer_->avail())
    {
        currentbuffer_->append(data, length);
    }
    else
    {
        buffers_.push_back(std::move(currentbuffer_));
        if (nextbuffer_)
            currentbuffer_ = std::move(nextbuffer_);
        else
            currentbuffer_.reset(new Buffer());
        currentbuffer_->append(data, length);
        cond_.notify_one();
    }
}

void AsyncLogging::threadFunc()
{

    BufferPtr newbuffer1(new Buffer());
    BufferPtr newbuffer2(new Buffer());
    BufferVector buffersToWrite;
    while (1)
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            if (buffers_.empty())
            {
                printf("wait for three seconds \n");
                fflush(stdout);
                cond_.wait_for(lock, std::chrono::seconds(flushInterval_));
            }
            buffers_.push_back(std::move(currentbuffer_));
            buffers_.swap(buffersToWrite);
            currentbuffer_ = std::move(newbuffer1);
            if (!nextbuffer_)
                nextbuffer_ = std::move(newbuffer2);
        }
        if (buffersToWrite.size() > 25)
        {
            // char buf[256];
            // snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
            //          Timestamp::now().toFormattedString().c_str(),
            //          buffersToWrite.size() - 2);
            // fputs(buf, stderr);
            // output.append(buf, static_cast<int>(strlen(buf)));
            buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
        }
        for (const auto &buffer : buffersToWrite)
        {
            printf("%s \n",buffer->data());
        }
        if (buffersToWrite.size() > 2) //避免不必要的resize
            buffersToWrite.resize(2);
        newbuffer1 = std::move(buffersToWrite.back());
        buffersToWrite.pop_back();
        newbuffer1->reset();
        if (newbuffer2 == NULL)
        {
            newbuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newbuffer2->reset();
        }

        // outflush();
    }
}
void AsyncLogging::start()
{
    thread_ = std::thread(&AsyncLogging::threadFunc, this);
}

void AsyncLogging::stop()
{
    thread_.join();
}