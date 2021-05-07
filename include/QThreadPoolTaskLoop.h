#ifndef _Q_THREAD_POOL_TASK_LOOP_H_
#define _Q_THREAD_POOL_TASK_LOOP_H_

#include"QTaskLoop.h"

struct QIThreadPoolTaskLoop : public QIUnknown
{
    virtual void setMaxThreadCount(int count) = 0;
    virtual int getMaxThreadCount() const = 0;
};

QT_DEFINE_CLSID(CLSID_ThreadPoolCommandLoop, "qtcom.class.threadpoolCommandLoop");
#endif
