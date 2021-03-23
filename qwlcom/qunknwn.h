#ifndef QUNKNWN_H
#define QUNKNWN_H

#include<QUuid>
#define STDMETHODCALL __stdcall
#ifdef _WIN32

#else
#define STDMETHODCALL
#endif

#define QS_OK                             (0L)
#define QS_FALSE                          (1L)
#define QE_INVALIDARG                     (0x80070057L)
#define QE_NOTIMPL                        (0x80004001L)
#define QE_NOINTERFACE                    (0x80004002L)


typedef QUuid IID;
typedef ulong QHRESULT;

#define QFAILED(hr) (((QHRESULT)(hr)) < 0)

template <class T>
inline const QUuid& _luuidof( )
{
    return QUuid();
}

#define QT_DEFINE_IID(iface, uuid_string)\
template<>\
inline const QUuid& _luuidof<iface>( ){\
    static QUuid guid = QUuid::fromString(QString::fromLatin1(uuid_string));\
    return guid;\
}

#define qt_uuidof(X) _luuidof<X>()

#define QT_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
     const QUuid  name \
    = { l, w1, w2,  b1, b2,  b3,  b4,  b5,  b6,  b7,  b8  }

struct QIUnknown
{
    virtual ulong STDMETHODCALL AddRef() = 0;
    virtual ulong STDMETHODCALL Release() = 0;
    virtual QHRESULT STDMETHODCALL QueryInterface(const QUuid& uuid, void  **ppvObject) = 0;
};
QT_DEFINE_IID(QIUnknown,"{00000000-0000-0000-C000-000000000046}");

#define QINull ((QIUnknown*)0)

struct QIClassFactory : public QIUnknown
{
public:
    virtual QHRESULT STDMETHODCALL CreateInstance( QIUnknown *pUnkOuter,const QUuid& riid, void **ppvObject) = 0;
    virtual QHRESULT STDMETHODCALL LockServer( bool fLock) = 0;
};
QT_DEFINE_IID(QIClassFactory,"{00000001-0000-0000-C000-000000000046}");


struct QIClassFactoryEx: public QIClassFactory
{
public:
    virtual QHRESULT   STDMETHODCALL CreateInstance(QIUnknown *prot, QIUnknown *punkOuter, const QUuid& riid, void **ppvObject) = 0;
    virtual QUuid STDMETHODCALL GetAt(int nIndex) = 0;
    virtual int   STDMETHODCALL GetCount() = 0;
    virtual QString STDMETHODCALL ProgIDFromCLSID(const QUuid& clsid) = 0;
};
QT_DEFINE_IID(QIClassFactoryEx,"{6966E385-DBFA-4131-A29E-D0E9464F3F53}");

// {E9678781-A3CB-46fb-9121-3ED22C24CFAD}
QT_DEFINE_GUID(CLSID_QClassFactory,
               0xe9678781, 0xa3cb, 0x46fb, 0x91, 0x21, 0x3e, 0xd2, 0x2c, 0x24, 0xcf, 0xad);

#endif // QUNKNWN_H
