#ifndef _QTASK_COMMAND_BASE_H_
#define _QTASK_COMMAND_BASE_H_
#include<QHash>
#include <QIUnknownImpl>
#include "QComPtr"
#include "QTaskLoop.h"
class QTaskCommandBase :
        public QITaskCommand,
        private QUnknownImp
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QITaskCommand)
        QTCOM_QUERYINTERFACE_ENTRY(QITaskCommand)
    QTCOM_QUERYINTERFACE_END

    QTaskCommandBase( void* parent = nullptr):m_priority(10)
    {
        Q_UNUSED(parent)
    }

    virtual ~QTaskCommandBase()
    {
        m_contexts.clear();
    }

    QHRESULT init_class( QIUnknown* , QIUnknown*)
    {
        return QS_OK;
    }

protected:
    virtual void setName(const QString& name){
        m_name = name;
    }

    virtual QString getName() const{
        return m_name;
    }

    virtual QErrorCode run() = 0;

    virtual void cancel(){
        QList<QtComPtr<QITaskCommandContext>>::iterator it = m_contexts.begin();
        for( ; it != m_contexts.end(); it++){
            QtComPtr<QITaskCommandContext> p = *it;
            if(p)
                p->cancel();
        }
    }

    virtual bool isCancel(){

        QList<QtComPtr<QITaskCommandContext>>::iterator it = m_contexts.begin();
        for( ; it != m_contexts.end(); it++){
            QtComPtr<QITaskCommandContext> p = *it;
            if(p && p->isCancel())
                return true;
        }

        return false;
    }

    virtual void addContext(QITaskCommandContext* context){
		
        if(context){
            m_contexts.append(context);
        }
    }


    virtual void removeContext(QITaskCommandContext* context){
        m_contexts.removeOne(context);
    }

    virtual void reslove(){

    }

    virtual void reject(const QErrorCode&) {

    }

    virtual void setPriority(int priority){

        m_priority = priority;
    }
    virtual int getPriority() const{
        return m_priority;
    }
private:
    QString m_name;
    QList<QtComPtr<QITaskCommandContext>> m_contexts;
    int m_priority;
};

#endif
