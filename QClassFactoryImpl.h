#ifndef QCLASSFACTORYIMPL_H
#define QCLASSFACTORYIMPL_H

#include"QIUnknownImpl.h"
#include"QIClassFactory.h"
#include"QtComPtr.h"

#ifdef Q_OS_WIN
#define QEXPORT_API __declspec(dllexport)
#else
#define QEXPORT_API
#endif

#define QTCOM_BEGIN_CLIDMAP \
extern "C" QHRESULT QEXPORT_API QtDllGetClassObject( const QCLSID& clsid, const QIID& iid, void** pCls)\
{\
    QtComPtr<QIUnknown> p; if( clsid.isNull() ) return QE_NOTIMPL;

#define QTCOM_END_CLIDMAP\
    if(p) return p->QueryInterface(iid, pCls); return QE_NOTIMPL;}

#define QTCOM_CLIDMAPENTRY( CID, CLASS) \
    else if(clsid == CID) p = new QTClsFactory<CLASS>();


template<class CLS>
class QTClsFactory : public QIClassFactory, public QIUnknownImpl
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIClassFactory)
    QTCOM_QUERYINTERFACE_END

    QTClsFactory()
    {
    }
public:
    virtual QHRESULT CreateInstance(const QIID& iid, void **ppv,QIUnknown *prot = QINull, QIUnknown *pUnkOuter = QINull)
    {
        QRASSERT(ppv, QE_INVALIDARG);

        *ppv = 0;
        CLS* p(new CLS());
        QRASSERT(p, QE_UNEXPECTED);
        QHRESULT hr = p->init_class(prot, pUnkOuter);
        if (QSUCCESSED(hr))
        {
            hr = p->QueryInterface(iid, ppv);
            if (QSUCCESSED(hr))
                return hr;
        }

        delete p;
        return hr;
    }
};


#endif
