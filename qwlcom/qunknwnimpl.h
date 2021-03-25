#ifndef QUNKNWNIMPL_H
#define QUNKNWNIMPL_H

#include<QAtomicInteger>
#include"qunknwn.h"
class QUnknownImp
{
public:
    QAtomicInteger<ulong> m_RefCount;
    QUnknownImp(): m_RefCount(0) {}
    virtual ~QUnknownImp(){}
};

#define ADDREF	\
    virtual ulong STDMETHODCALL AddRef()	\
        {return this->m_RefCount++; }

#define RELEASE	\
    virtual ulong STDMETHODCALL Release()	\
       { this->m_RefCount--; if(this->m_RefCount) return this->m_RefCount; delete this; return 0; }

#define ADDREF_NOREF	\
    virtual ulong STDMETHODCALL AddRef()	\
        {return 1L; }

#define RELEASE_NOREF	\
    virtual ulong STDMETHODCALL Release()	\
       { return 1L; }

#define QTCOM_ADDREF_RELEASE ADDREF RELEASE

#define QTCOM_ADDREF_RELEASE_NOREF ADDREF_NOREF RELEASE_NOREF

#define QTCOM_QUERYINTERFACE_ENTRY(i) \
    else if(qt_uuidof(i) == iid) { *ppv = static_cast<i*>(this); AddRef(); return QS_OK; }


#define QTCOM_QUERYINTERFACE_BEGIN(icast) \
    QSTDMETHOD(QueryInterface)(const QIID& iid, void **ppv) {\
    if(qt_uuidof(QIUnknown) == iid) { *ppv = static_cast<QIUnknown*>(static_cast<icast*>(this)); AddRef(); return QS_OK; }\

#define QTCOM_QUERYINTERFACE_END \
            return QE_NOINTERFACE; }


#endif // QUNKNWNIMPL_H
