#ifndef QCLASSOBJECTS_H
#define QCLASSOBJECTS_H
#include <QHash>
#include<QMutex>
#include<QLibrary>
#include "qunknwnimpl.h"
#include"qcomlibrary.h"
#include"qcomptr.h"


class QClassObjectsContainer : public QIClassObjectContainer , private QUnknownImp
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIClassObjectContainer)
        QTCOM_QUERYINTERFACE_ENTRY(QIClassObjectContainer)
    QTCOM_QUERYINTERFACE_END

    QHRESULT init_class( QIUnknown*, QIUnknown*)
    {
        return QS_OK;
    }

    ~QClassObjectsContainer()
    {
        RevokeAll();
    }

    QSTDMETHOD(CreateInstance)(const QCLSID& clsid, QIUnknown *prot, QIUnknown *punkOuter, const QIID& iid, void **ppvObject)
    {
        QComPtr<QIClassFactoryEx> pCls;
        QRFAILED(GetClassObject(clsid, qt_uuidof(QIClassFactoryEx), (void**)&pCls.m_p));
        return pCls->CreateInstance(prot, punkOuter, iid, ppvObject);
    }

    QSTDMETHOD(GetClassObject)(const QCLSID& clsid, const QIID& iid,void **ppv)
    {
        QMutexLocker locker(&m_mutex);
        if( !m_clsobjs.contains(clsid))
            return QE_NOTFIND;

        QComLibrary* lib = m_clsobjs[clsid];
        if( lib )
            return lib->DllGetClassObject(clsid, iid, ppv);

        return QE_UNEXPECTED;
    }

    QSTDMETHOD(Register)(const QCLSID& clsid, const QString& module)
    {
        QMutexLocker locker(&m_mutex);
        if( m_clsobjs.contains(clsid))
            return QE_EXIST;

        QComLibrary* lib = new QComLibrary();
        if( !lib )
            return QE_UNEXPECTED;

        if(lib->open(module) == QS_OK)
        {
            m_clsobjs.insert(clsid, lib);
            return QS_OK;
        }

        delete  lib;
        return QE_FAIL;
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

    QSTDMETHOD(RevokeAll)()
    {
        QMutexLocker locker(&m_mutex);
        QHash<QCLSID, QComLibrary*>::const_iterator i;
        for (i = m_clsobjs.begin(); i != m_clsobjs.end(); ++i)
        {
            QComLibrary* lib = i.value();
            if( lib )
                delete lib;
        }

        m_clsobjs.clear();
        return QS_OK;
    }

private:
    QMutex m_mutex;
    QHash<QCLSID, QComLibrary*> m_clsobjs;
};

#endif // QCLASSOBJECTS_H
