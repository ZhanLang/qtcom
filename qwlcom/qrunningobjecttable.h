#ifndef QRUNNINGOBJECTTABLE_H
#define QRUNNINGOBJECTTABLE_H

#include<QMap>
#include<QMutex>

#include"qunknwnimpl.h"
#include"qcomptr.h"

class QRunningObjectTableImpl: public QIRunningObjectTable, public QUnknownImp
{
public:

    UNKNOWN_IMP1_(QIRunningObjectTable);
    QHRESULT init_class( QIUnknown*, QIUnknown*)
    {
        return QS_OK;
    }

    QSTDMETHOD(QuickCreateInstance)(const QCLSID& clsid, QIUnknown *punkOuter, const QIID& iid, void **ppvObject)
    {
        QComPtr<QIClassObjectContainer> pClsobjs;
        QHRESULT hr = GetObject(clsid, qt_uuidof(QIClassObjectContainer), (void**)&pClsobjs);
        if( hr != QS_OK)
            return hr;

        hr = pClsobjs->isRegistered( clsid );
        if( hr != QS_OK )
            return hr;

        return pClsobjs->CreateInstance(clsid,
                                        static_cast<QIUnknown*>(static_cast<QIRunningObjectTable*>(this)),
                                        punkOuter, iid, ppvObject);
    }

    QSTDMETHOD(GetObject)(const QCLSID& clsid, const QIID& iid, void **ppunk)
    {
        QMutexLocker locker(&m_mutex);
        if( !m_objs.contains(clsid))
            return QE_NOTFIND;

       return m_objs[clsid]->QueryInterface(iid, ppunk);
    }

    QSTDMETHOD(Register)(const QCLSID& clsid, QIUnknown* punk)
    {
        QMutexLocker locker(&m_mutex);
        if(m_objs.contains(clsid))
            return QE_EXIST;

        m_objs.insert(clsid, punk);
        return QS_OK;
    }

    QSTDMETHOD_(bool, isRunning)(const QCLSID& clsid)
    {
        QMutexLocker locker(&m_mutex);
        return m_objs.contains(clsid);
    }

    QSTDMETHOD(Revoke)(const QCLSID& clsid)
    {
        QMutexLocker locker(&m_mutex);
        if(!m_objs.contains(clsid))
            return QE_NOTFIND;

        m_objs.remove(clsid);
        return QS_OK;
    }

    QSTDMETHOD(RevokeAll)()
    {
        QMutexLocker locker(&m_mutex);
        m_objs.clear();
        return QS_OK;
    }

private:
    QMutex m_mutex;
    QMap<QCLSID, QComPtr<QIUnknown>> m_objs;
};

#endif // QRUNNINGOBJECTTABLE_H
