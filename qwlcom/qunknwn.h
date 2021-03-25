#ifndef QUNKNWN_H
#define QUNKNWN_H

#include<QUuid>
#include<QVariant>
#define STDMETHODCALL __stdcall
#ifdef _WIN32

#else
#define STDMETHODCALL
#endif

#define QS_OK                             (0L)
#define QS_FALSE                          (1L)
#define QE_FAIL                           (0x80004005L)
#define QE_INVALIDARG                     (0x80070057L)
#define QE_NOTIMPL                        (0x80004001L)
#define QE_NOINTERFACE                    (0x80004002L)
#define QE_UNEXPECTED                     (0x8000FFFFL)
#define QE_NOTFIND                        (0x60004001L)
#define QE_EXIST                          (0x60004002L)
typedef QUuid QIID, QCLSID;
typedef ulong QHRESULT;

#define QFAILED(hr) (((QHRESULT)(hr)) < 0)
#define QRASSERT(x, _h_r_) { if(!(x)) return _h_r_; }
#define QRFAILED(x) { QHRESULT _h_r_ = (x); if(QFAILED(_h_r_)) return _h_r_; }

#define QSTDMETHOD(method) virtual QHRESULT STDMETHODCALL method
#define QSTDMETHOD_(result,method) virtual result STDMETHODCALL method

template <class T>
inline const QIID& _luuidof( )
{
    static QIID iid = QIID();
    return iid;
}

#define QT_DEFINE_IID(iface, uuid_string)\
template<>\
inline const QIID& _luuidof<iface>(){\
    static QIID guid = QUuid::fromString(QString::fromLatin1(uuid_string));\
    return guid;\
}

#define qt_uuidof(X) _luuidof<X>()

#define QT_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
     const QUuid  name \
    = { l, w1, w2,  b1, b2,  b3,  b4,  b5,  b6,  b7,  b8  }

struct QIUnknown
{
    QSTDMETHOD_(ulong,AddRef)() = 0;
    QSTDMETHOD_(ulong,Release)() = 0;
    QSTDMETHOD(QueryInterface)(const QIID& uuid, void  **ppvObject) = 0;
};
QT_DEFINE_IID(QIUnknown,"{00000000-0000-0000-C000-000000000046}");

#define QINull ((QIUnknown*)0)

struct QIRunningObjectTable : public QIUnknown
{
    QSTDMETHOD(QuickCreateInstance)(const QCLSID& clsid, QIUnknown *punkOuter, const QIID& iid, void **ppvObject) = 0;

    QSTDMETHOD(GetObject)(const QCLSID& clsid, const QIID& iid, void **ppunk) = 0;

    QSTDMETHOD(Register)(const QCLSID& clsid, QIUnknown* punk) = 0;
    QSTDMETHOD_(bool, isRunning)(const QCLSID& clsid) = 0;

    QSTDMETHOD(Revoke)(const QCLSID& clsid) = 0;
    QSTDMETHOD(RevokeAll)() = 0;
};
QT_DEFINE_IID(QIRunningObjectTable,"{DBAF323F-767A-452E-A779-FAAFD1318DE7}");

// {C0BE31C1-EE06-4F2E-A8C9-97B6D13A7790}
QT_DEFINE_GUID(CLSID_RunningObjectTable,
    0xc0be31c1, 0xee06, 0x4f2e, 0xa8, 0xc9, 0x97, 0xb6, 0xd1, 0x3a, 0x77, 0x90);


struct QIClassObjectContainer : public QIUnknown
{
    QSTDMETHOD(CreateInstance)(const QCLSID& clsid, QIUnknown *prot, QIUnknown *punkOuter, const QIID& iid, void **ppvObject) = 0;

    QSTDMETHOD(GetClassObject)(const QCLSID& clsid,const QIID& iid, void **pCls) = 0;

    QSTDMETHOD(Register)(const QCLSID& clsid, const QString& module) = 0;
    QSTDMETHOD_(bool,isRegistered)(const QCLSID& clsid) = 0;

    QSTDMETHOD(Revoke)(const QCLSID& clsid) = 0;
    QSTDMETHOD(RevokeAll)() = 0;
};
QT_DEFINE_IID(QIClassObjectContainer,"{E6854E4D-5549-4D33-B8A5-B39F6786531C}");

// {1032D791-B0FC-41B5-9523-614450DF366C}
QT_DEFINE_GUID(CLSID_ClassObjects,
    0x1032d791, 0xb0fc, 0x41b5, 0x95, 0x23, 0x61, 0x44, 0x50, 0xdf, 0x36, 0x6c);

struct QIApplication : public QIUnknown
{
    QSTDMETHOD_(QVariant,GetProperty)(const QString& key) = 0;
    QSTDMETHOD(SetProperty)(const QString& key, const QVariant& value) = 0;

    QSTDMETHOD(Exec)() = 0;
    QSTDMETHOD(Quit)(int returnCode) = 0;
};
QT_DEFINE_IID(QIApplication,"{24d0c25e-632c-4440-b057-68ed5bb415f5}");

struct QIPlugin : public QIUnknown
{
    QSTDMETHOD(Init)(void* ) = 0;
    QSTDMETHOD(UnInit)(int returnCode) = 0;
};
QT_DEFINE_IID(QIPlugin,"{1f06f05a-1bb1-4ded-b0b2-473865875dbf}");

struct QIPluginRun : public QIUnknown
{
    QSTDMETHOD(Start)() = 0;
    QSTDMETHOD(Stop)() = 0;
};
QT_DEFINE_IID(QIPluginRun,"{c1bdb92b-444c-4750-a4a4-7bfe1e2ff74f}");

struct QIClassFactory : public QIUnknown
{
public:
   QSTDMETHOD(CreateInstance)( QIUnknown *pUnkOuter,const QIID& iid, void **ppvObject) = 0;
   QSTDMETHOD(LockServer)( bool fLock) = 0;
};
QT_DEFINE_IID(QIClassFactory,"{00000001-0000-0000-C000-000000000046}");


struct QIClassFactoryEx: public QIUnknown
{
public:
    QSTDMETHOD(CreateInstance)(QIUnknown *prot, QIUnknown *punkOuter, const QIID& iid, void **ppvObject) = 0;
};
QT_DEFINE_IID(QIClassFactoryEx,"{6966E385-DBFA-4131-A29E-D0E9464F3F53}");

#endif // QUNKNWN_H
