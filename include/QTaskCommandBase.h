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

    QTaskCommandBase( void* parent = nullptr):m_priority(10),m_cancel(false)
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
        m_cancel = true;

        QHash<QString, QtComPtr<QITaskCommandContext>>::iterator it = m_contexts.begin();
        for( ; it != m_contexts.end(); it++){
            QtComPtr<QITaskCommandContext> p = it.value();
            if(p)
                p->cancel();
        }
    }

    virtual bool isCancel(){
        if( m_cancel )
            return m_cancel;

        QHash<QString, QtComPtr<QITaskCommandContext>>::iterator it = m_contexts.begin();
        for( ; it != m_contexts.end(); it++){
            QtComPtr<QITaskCommandContext> p = it.value();
            if(p && p->isCancel())
                return true;
        }

        return false;
    }

    virtual void setContext(const QString& id, QITaskCommandContext* context){
		
        if(context){
            m_contexts.insert(id,context);
        }
    }

    virtual void getContext(const QString& id, QITaskCommandContext** context){
		
        QHash<QString, QtComPtr<QITaskCommandContext>>::iterator it = m_contexts.find(id);
        if( it != m_contexts.end()){
            it.value()->QueryInterface(qt_uuidof(QITaskCommandContext), (void**)context);
        }
    }

    virtual void removeContext(const QString& id){
        m_contexts.remove(id);
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
    QHash<QString, QtComPtr<QITaskCommandContext>> m_contexts;

    int m_priority;
    bool m_cancel;
};

#endif
