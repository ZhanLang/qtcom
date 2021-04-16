#ifndef QCLASSOBJECTS_H
#define QCLASSOBJECTS_H
#include <QHash>
#include<QMutex>
#include<QLibrary>
#include "qunknwnimpl.h"
#include"qcomlibrary.h"
#include"qcomptr.h"


class QClassContainer : public QIClassContainer , private QUnknownImp
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIClassContainer)
        QTCOM_QUERYINTERFACE_ENTRY(QIClassContainer)
    QTCOM_QUERYINTERFACE_END

    QClassContainer(void* parent)
    {
        Q_UNUSED(parent)
    }

    QHRESULT init_class( QIUnknown*, QIUnknown*)
    {
        return QS_OK;
    }

    ~QClassContainer()
    {
        RevokeAll();
    }



    QSTDMETHOD(GetClassObject)(const QCLSID& clsid, const QIID& iid,void **ppv)
    {
        QMutexLocker locker(&m_mutex);
        if( !m_clsobjs.contains(clsid))
            return QE_NOTFIND;

        QComLibrary* lib = m_clsobjs[clsid];
        if( !lib )
            return QE_RUNTIME;

        if( !lib->isLoaded() && !lib->open())
        {
             //todo log
             return QE_UNEXPECTED;
        }

        return lib->DllGetClassObject(clsid, iid, ppv);
    }

    QSTDMETHOD(Register)(const QCLSID& clsid, const QString& path, const QString& file)
    {
        QMutexLocker locker(&m_mutex);
        if( m_clsobjs.contains(clsid))
            return QE_EXIST;

        if(!QLibrary::isLibrary(path))
        {
            //todo log
            return QE_UNEXPECTED;
        }

        QComLibrary* lib = new QComLibrary();
        if( !lib )
            return QE_UNEXPECTED;

        lib->setFileName(path);
        m_clsobjs.insert(clsid, lib);

        return QS_OK;
    }

    QSTDMETHOD_(bool,isRegistered)(const QCLSID& clsid)
    {
        QMutexLocker locker(&m_mutex);
        return m_clsobjs.contains(clsid);
    }

    QSTDMETHOD(Revoke)(const QCLSID& clsid)
    {
        QMutexLocker locker(&m_mutex);
        if( !m_clsobjs.contains(clsid))
            return QE_NOTFIND;

        QComLibrary* lib = m_clsobjs[clsid];
        if( lib )
            delete lib;

        m_clsobjs.remove(clsid);
        return QS_OK;
    }

    QSTDMETHOD_(void,RevokeAll)()
    {
        QMutexLocker locker(&m_mutex);
        QHash<QCLSID, QComLibrary*>::iterator i = m_clsobjs.begin();
        for (; i != m_clsobjs.end(); ++i)
        {
            QComLibrary* lib = i.value();
            if( lib )
                delete lib;
        }

        m_clsobjs.clear();
    }

private:
    QMutex m_mutex;
    QHash<QCLSID, QComLibrary*> m_clsobjs;
};

#endif // QCLASSOBJECTS_H
