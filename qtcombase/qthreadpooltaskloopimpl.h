#ifndef QTHREADPOOLTASKLOOPIMPL_H
#define QTHREADPOOLTASKLOOPIMPL_H

#include<QThreadPoolTaskLoop.h>
#include<QIUnknownImpl>
#include<QThreadPool>
#include<QList>
#include<QComPtr>


class QThreadPoolTaskLoopImpl :
        public QIThreadPoolTaskLoop ,
        public QITaskLoop,
        private QUnknownImp,
        private QThreadPool
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIThreadPoolTaskLoop)
        QTCOM_QUERYINTERFACE_ENTRY(QIThreadPoolTaskLoop)
        QTCOM_QUERYINTERFACE_ENTRY(QITaskLoop)
    QTCOM_QUERYINTERFACE_END

    QThreadPoolTaskLoopImpl(void*);
    QHRESULT init_class( QIUnknown* ,QIUnknown*)
    {
        return QS_OK;
    }

protected:
    virtual void setMaxThreadCount(int count);
    virtual int getMaxThreadCount() const;

protected:
    virtual void exec(QITaskCommand* task);
    virtual int count();
    virtual void close();

private:
    QtComPtr<QITaskCommandContext> m_context;
};

#endif // QTHREADPOOLTASKLOOPIMPL_H
