#ifndef _QTASK_LOOP_H_
#define _QTASK_LOOP_H_

#include<QIUnknown>
#include<QErrorCode.h>

struct QITaskCommandContext: public QIUnknown
{
    virtual bool isCancel() const = 0;
    virtual void cancel() = 0;
};

QT_DEFINE_IID(QITaskCommandContext,"qtcom.i.taskloopContext");


struct QITaskCommand: public QIUnknown
{
    virtual void setName(const QString& name) = 0;
    virtual QString getName() const = 0;

    virtual QErrorCode run() = 0;
    virtual bool isCancel() = 0;

    virtual void setContext(const QString& id, QITaskCommandContext* context) = 0;
    virtual void getContext(const QString& id, QITaskCommandContext** context);
    virtual void removeContext(const QString& id);

    virtual void reslove() = 0;
    virtual void reject(const QErrorCode& code) = 0;

    virtual void setPriority(int priority) = 0;
    virtual int getPriority() const = 0 ;
};
QT_DEFINE_IID(QITaskCommand,"qtcom.i.taskcommand")


struct QITaskLoop : public QIUnknown
{
    virtual void exec(QITaskCommand* task) = 0;
    virtual int count() = 0;
    virtual void close() = 0;
};

QT_DEFINE_IID(QITaskLoop,"qtcom.i.taskloop");


#endif

