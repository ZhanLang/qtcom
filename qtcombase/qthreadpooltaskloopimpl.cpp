#include "qthreadpooltaskloopimpl.h"

#include<QDebug>

class QTaskQRunnable :public QRunnable
{
public:
    QTaskQRunnable(QITaskCommand* cmd)
        :m_cmd(cmd),
          m_e(QErrorCode::Canceled)
    {
        setAutoDelete(true);
    }

    virtual ~QTaskQRunnable()
    {
        if (m_cmd)
        {
            if (!m_e)
                m_cmd->reject(m_e);
            else
                m_cmd->reslove();
        }

        m_cmd = QINull;
    }

    virtual void run()
    {
        if (!m_cmd)
            return;


        if (m_cmd->isCancel())
        {
            qWarning() << QString("task command cancel %1 .").arg(m_cmd->getName());
            return;
        }

        try
        {
            m_e = m_cmd->run();
            if ( !m_e)
            {
                QString msg = QString("exec command %1 faild.").arg(m_cmd->getName());
                qWarning() << msg;
            }
        }
        catch (const std::bad_alloc & e)
        {
            std::string w = e.what();
            w += " QTaskCommandLoop run exception one.";
            qWarning() << m_cmd->getName() << " : " << w.c_str();

            m_e = QErrorCode(QErrorCode::CppException, e.what());
        }
        catch (const std::exception& e)
        {
            std::string w = e.what();
            w += " QTaskCommandLoop run exception one.";
            qWarning() << m_cmd->getName() << " : " << w.c_str();

            m_e = QErrorCode(QErrorCode::CppException, w.c_str());
        }
        catch (...)
        {
            m_e = QErrorCode(QErrorCode::CppException, "QTaskCommandLoop run exception two.");
        }

    }
private:
    QtComPtr<QITaskCommand> m_cmd;
    QErrorCode m_e;
};

class QThreadPoolTaskContext :
        public QITaskCommandContext,
        private QUnknownImp
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QITaskCommandContext)
        QTCOM_QUERYINTERFACE_ENTRY(QITaskCommandContext)
    QTCOM_QUERYINTERFACE_END

    QHRESULT init_class( QIUnknown* ,QIUnknown*)
    {
        return QS_OK;
    }
    QThreadPoolTaskContext():m_cancel(false)
    {

    }

    void cancel()
    {
        m_cancel = true;
    }

    virtual bool isCancel() const
    {
        return m_cancel;
    }

private:
    bool m_cancel;
};

QThreadPoolTaskLoopImpl::QThreadPoolTaskLoopImpl(void*)
{
    m_context = new QThreadPoolTaskContext();
}

void QThreadPoolTaskLoopImpl::setMaxThreadCount(int count)
{
    return QThreadPool::setMaxThreadCount(count);
}

int QThreadPoolTaskLoopImpl::getMaxThreadCount() const
{
    return QThreadPool::maxThreadCount();
}


void QThreadPoolTaskLoopImpl::exec(QITaskCommand *task)
{
    if( !task )
        return;

    task->addContext(m_context);

    QTaskQRunnable* r = new QTaskQRunnable(task);
    if ( r )
         QThreadPool::start(r, task->getPriority());
}

void QThreadPoolTaskLoopImpl::close()
{
    m_context->cancel();
    QThreadPool::waitForDone();
}

int QThreadPoolTaskLoopImpl::count()
{
    return -1;
}
