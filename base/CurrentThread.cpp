#include "CurrentThread.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/types.h>

using namespace muduo;

__thread int CurrentThread::t_cachedTid;
__thread char CurrentThread::t_tidString[32];
__thread int CurrentThread::t_tidStringLength;
__thread const char *CurrentThread::t_threadName;

void CurrentThread::cacheTid()
{
    t_cachedTid = static_cast<pid_t>(syscall(SYS_gettid));
    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d", t_cachedTid);
}