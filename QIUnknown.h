#ifndef _QUNKNWN_H_
#define _QUNKNWN_H_

#include <QString>
#include <QVariant>
#define STDMETHODCALL __stdcall
#ifdef _WIN32

#else
#define STDMETHODCALL
#endif

#define QS_OK                             (0L)
#define QS_FALSE                          (1L)
#define QE_FAIL                           (3000L)
#define QE_INVALIDARG                     (3001L)
#define QE_NOTIMPL                        (3002L)
#define QE_NOINTERFACE                    (3003L)
#define QE_UNEXPECTED                     (3004L)
#define QE_NOTFIND                        (3005L)
#define QE_EXIST                          (3006L)
#define QE_RUNTIME                        (3007L)
#define QE_NOROT                          (3008L)
#define QE_POINTER                        (3009L)

typedef QString QIID, QCLSID;
typedef long QHRESULT;

#define QIID_NULL ""

#define QFAILED(hr) (hr != QS_OK)
#define QSUCCESSED(hr) (hr == QS_OK)

#define QRASSERT(x, _h_r_) { if(!(x)) return _h_r_; }
#define QRFAILED(x) { QHRESULT _h_r_ = (x); if(QFAILED(_h_r_)) return _h_r_; }

#define QSTDMETHOD(method) virtual QHRESULT STDMETHODCALL method
#define QSTDMETHOD_IMPL QHRESULT STDMETHODCALL

#define QSTDMETHOD_(result,method) virtual result STDMETHODCALL method
#define QSTDMETHOD_IMPL_(result) result STDMETHODCALL

template <class T>
inline const QIID& _luuidof( )
{
    static QIID iid = QIID();
    return iid;
}

#define QT_DEFINE_IID(iface, uuid_string)\
template<>\
inline const QIID& _luuidof<iface>(){\
    static QIID guid = uuid_string;\
    return guid;\
}


#define qt_uuidof(X) _luuidof<X>()

#define QT_DEFINE_CLSID(name, clsid_string) \
     static const QString name = clsid_string;\

struct QIUnknown
{
    virtual ulong AddRef() = 0;
    virtual ulong Release() = 0;
    virtual QHRESULT QueryInterface(const QIID& uuid, void  **ppvObject) = 0;
};
QT_DEFINE_IID(QIUnknown,"qtcom.i.unknow");

#define QINull ((QIUnknown*)0)


#define ADDREF	\
    virtual ulong AddRef()	\
        {return this->m_RefCount++; }

#define RELEASE	\
    virtual ulong Release()	\
       { this->m_RefCount--; if(this->m_RefCount) return this->m_RefCount; delete this; return 0; }

#define QT_RELEASE	\
    virtual ulong Release()	\
       { this->m_RefCount--; if(this->m_RefCount) return this->m_RefCount; this->deleteLater(); return 0; }

#define ADDREF_NOREF	\
    virtual ulong AddRef()	\
        {return 1L; }

#define RELEASE_NOREF	\
    virtual ulong Release()	\
       { return 1L; }

#define QTCOM_ADDREF_RELEASE ADDREF RELEASE
#define QTCOM_ADDREF_QTRELEASE ADDREF QT_RELEASE
#define QTCOM_ADDREF_RELEASE_NOREF ADDREF_NOREF RELEASE_NOREF

#define QTCOM_QUERYINTERFACE_ENTRY(i) \
    else if(qt_uuidof(i) == iid) { *ppv = static_cast<i*>(this); AddRef(); return QS_OK; }


#define QTCOM_QUERYINTERFACE_BEGIN(i) \
    QIUnknown* toIUnknown() {return static_cast<QIUnknown*>(static_cast<i*>(this));}\
    virtual QHRESULT QueryInterface(const QIID& iid, void **ppv) {\
    if(qt_uuidof(QIUnknown) == iid) { *ppv = toIUnknown(); AddRef(); return QS_OK; }\
    else if(qt_uuidof(i) == iid) { *ppv = static_cast<i*>(this); AddRef(); return QS_OK; }

#define QTCOM_QUERYINTERFACE_END \
            return QE_NOINTERFACE; }

#endif
