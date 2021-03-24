#ifndef QCLASSFACTORYIMPL_H
#define QCLASSFACTORYIMPL_H

#include"qunknwnimpl.h"
#include"qcomptr.h"

#ifdef Q_OS_WIN
#define QEXPORT_API __declspec(dllexport)
#else
#define QEXPORT_API
#endif

#define QBEGIN_CLIDMAP \
extern "C" QHRESULT QEXPORT_API DllGetClassObject( const QCLSID& clsid, const QIID& iid, void** pCls)\
{\
    QComPtr<QIUnknown> p; if( clsid.isNull() ) return QE_NOTIMPL;

#define QEND_CLIDMAP\
    if(p) return p->QueryInterface(iid, pCls); return QE_NOTIMPL;}

#define QCLIDMAPENTRY( CID, CLASS) \
    else if(clsid == CID) p = new QTStdClsFactory<CLASS>();

template<class CLS, class IFactory = QIClassFactory>
class QTClsFactory : public IFactory, public QUnknownImp
{
public: // QIUnknown:
    UNKNOWN_IMP1_(IFactory)
public:

    // QIClassFactory
    virtual QHRESULT STDMETHODCALL CreateInstance(
        /* [unique][in] */ QIUnknown *punkOuter,
        /* [in] */ const QUuid& riid,
        /* [iid_is][out] */ void **ppv)
    {
        return create_instance(punkOuter, riid, ppv);
    }

    virtual QHRESULT STDMETHODCALL LockServer(bool fLock)
    {
        return lock_server(fLock);
    }

public:
    static QHRESULT create_instance(QIUnknown *punkOuter, const QIID& riid, void **ppv)
    {
        QRASSERT(ppv, QE_INVALIDARG);


        *ppv = 0;
        QRASSERT(!punkOuter || qt_uuidof(QIUnknown) == riid, QE_INVALIDARG);
        QComPtr<CLS> p(new CLS);
        QRASSERT(p, QE_UNEXPECTED);
        QRFAILED(p->init_class(QINull, punkOuter));
        QRFAILED(p->QueryInterface(riid, ppv));
        p.detach();
        return QS_OK;
    }

    static QHRESULT lock_server(bool /*fLock*/)
    {
        return QS_OK;
    }

    static QHRESULT GetClassObject(const QIID& riid, void **ppv)
    {
        return QTClsFactory<QTClsFactory<CLS> >::create_instance(0, riid, ppv);
    }
};



template<class CLS>
class QTStdClsFactory : public QTClsFactory<CLS, QIClassFactoryEx>
{
public:
    virtual QHRESULT STDMETHODCALL CreateInstance(QIUnknown *prot, QIUnknown *punkOuter, const QIID& riid, void **ppv)
    {
        return create_instance(prot, punkOuter, riid, ppv);
    }

    // std factory invoke:
    virtual QHRESULT STDMETHODCALL init_class(QIUnknown* prot, QIUnknown* punkOuter)
    {
        Q_UNUSED(prot);
        return !punkOuter ? QS_OK : QE_INVALIDARG;
    }

    virtual QHRESULT STDMETHODCALL init_class_inner(QIUnknown* punkOuter)
    {
        return !punkOuter ? QS_OK : QE_INVALIDARG;
    }

public:
    static QHRESULT create_instance(QIUnknown *prot, QIUnknown *punkOuter, const QIID& riid, void **ppv)
    {
        QRASSERT(ppv, QE_INVALIDARG);
        *ppv = 0;
        QRASSERT(!punkOuter || qt_uuidof(QIUnknown) == riid, QE_INVALIDARG);
        sentry<CLS*> p(new CLS);
        QRASSERT(p, QE_UNEXPECTED);
        QRFAILED(p->init_class(prot, punkOuter));
        QRFAILED(p->QueryInterface(riid, ppv));
        p.detach();
        return QS_OK;
    }

    static QHRESULT GetClassObject(const QIID& riid, void **ppv)
    {
        return QTStdClsFactory<QTStdClsFactory<CLS> >::create_instance(0, 0, riid, ppv);
    }
};


#endif