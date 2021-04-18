#ifndef QUNKNWN_H
#define QUNKNWN_H

#include<QVariant>
#define STDMETHODCALL __stdcall
#ifdef _WIN32

#else
#define STDMETHODCALL
#endif

#define QS_OK                             (0L)
#define QE_FAIL                           (3000L)
#define QE_INVALIDARG                     (3001L)
#define QE_NOTIMPL                        (3002L)
#define QE_NOINTERFACE                    (3003L)
#define QE_UNEXPECTED                     (3004L)
#define QE_NOTFIND                        (3005L)
#define QE_EXIST                          (3006L)
#define QE_RUNTIME                        (3007L)
#define QE_NOROT                          (3008L)

typedef QString QIID, QCLSID;
typedef long QHRESULT;

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
     static const QString name = QString::fromLatin1(clsid_string);\

struct QIUnknown
{
    QSTDMETHOD_(ulong,AddRef)() = 0;
    QSTDMETHOD_(ulong,Release)() = 0;
    QSTDMETHOD(QueryInterface)(const QIID& uuid, void  **ppvObject) = 0;
};
QT_DEFINE_IID(QIUnknown,"qtcom.i.unknow");

#define QINull ((QIUnknown*)0)

struct QIRunningObjectTable : public QIUnknown
{
    QSTDMETHOD(GetObject)(const QCLSID& clsid, const QIID& iid, void **ppunk) = 0;

    QSTDMETHOD(Register)(const QCLSID& clsid, QIUnknown* punk) = 0;
    QSTDMETHOD_(bool, isRunning)(const QCLSID& clsid) = 0;

    QSTDMETHOD(Revoke)(const QCLSID& clsid) = 0;
    QSTDMETHOD(RevokeAll)() = 0;
};
QT_DEFINE_IID(QIRunningObjectTable,"qtcom.i.running_object_table");

QT_DEFINE_CLSID(CLSID_QRunningObjectTable,"qcom.class.running_object_table");


struct QIClassContainer : public QIUnknown
{
    QSTDMETHOD(CreateInstance)(const QCLSID& clsid,const QIID& iid, void **ppv, QIUnknown *prot = QINull,void* parent=nullptr, QIUnknown *pUnkOuter = QINull) = 0;

    QSTDMETHOD(GetClassObject)(const QCLSID& clsid,const QIID& factoryiid, void **pCls) = 0;

    QSTDMETHOD(Register)(const QCLSID& clsid, const QString& path, const QString& libName) = 0;

    QSTDMETHOD(registerModules)(const QByteArray& cfg) = 0;
    QSTDMETHOD(registerModulesFile)(const QString& file) = 0;
    QSTDMETHOD(registerModulesFiles)(const QStringList& files) = 0;

    QSTDMETHOD_(bool,isRegistered)(const QCLSID& clsid) = 0;

    QSTDMETHOD(Revoke)(const QCLSID& clsid) = 0;
    QSTDMETHOD_(void,RevokeAll)() = 0;
};
QT_DEFINE_IID(QIClassContainer,"qtcom.i.class_container");

QT_DEFINE_CLSID(CLSID_QClassContainer,"qtcom.class.class_container");


struct QIPluginContainer : public QIUnknown
{
    QSTDMETHOD(registerPlugins)(const QByteArray& cfg) = 0;
    QSTDMETHOD(registerPluginsFile)(const QString& file) = 0;
    QSTDMETHOD(registerPluginsFiles)(const QStringList& files) = 0;

    QSTDMETHOD(initPlugins)() = 0;
    QSTDMETHOD(unInitPlugins)() = 0;

    QSTDMETHOD(startPlugins)() = 0;
    QSTDMETHOD(stopPlugins)(int returnCode) = 0;
};
QT_DEFINE_IID(QIPluginContainer,"qtcom.i.plugin_container");
QT_DEFINE_CLSID(CLSID_QPluginContainer, "qtcom.class.plugin_container")

struct QIPropertySet : public QIUnknown
{
    QSTDMETHOD_(QVariant,getProperty)(const QString& key) = 0;
    QSTDMETHOD(addProperty)(const QString& key, const QVariant& value) = 0;
    QSTDMETHOD(addProperty)(const QMap<QString, QVariant>& values) = 0;

    QSTDMETHOD_(bool,isPropertyExist)(const QString& key) = 0;
    QSTDMETHOD_(QStringList,getPropertyKeys)() = 0;
};
QT_DEFINE_IID(QIPropertySet,"qtcom.i.propertyset");
QT_DEFINE_CLSID(CLSID_QPropertySet, "qtcom.class.propertyset")

struct QIApplication : public QIUnknown
{
    QSTDMETHOD(setConfigureFile)(const QString& cfgfile) = 0;
    QSTDMETHOD(Exec)(int argc, char *argv[]) = 0;
    QSTDMETHOD(Quit)(int returnCode) = 0;

    QSTDMETHOD_(QIPropertySet*,getPropertySet)() = 0;
    QSTDMETHOD_(QIRunningObjectTable*,getRunningObjectTable)() = 0;
};
QT_DEFINE_IID(QIApplication,"qtcom.i.applaction");

QT_DEFINE_CLSID(CLSID_QtApplaction, "qtcom.class.applaction")

struct QIPlugin : public QIUnknown
{
    QSTDMETHOD(Init)(QIPropertySet* ) = 0;
    QSTDMETHOD(UnInit)(int returnCode) = 0;
};
QT_DEFINE_IID(QIPlugin,"qtcom.i.plugin");

struct QIPluginRun : public QIUnknown
{
    QSTDMETHOD(Start)() = 0;
    QSTDMETHOD(Stop)() = 0;
};
QT_DEFINE_IID(QIPluginRun,"qtcom.i.plugin_run");

struct QIClassFactory : public QIUnknown
{
public:
   QSTDMETHOD(CreateInstance)(const QIID& iid, void **ppv, QIUnknown *prot = QINull,void* parent=nullptr, QIUnknown *pUnkOuter = QINull) = 0;
   QSTDMETHOD(LockServer)( bool fLock) = 0;
};
QT_DEFINE_IID(QIClassFactory,"qtcom.i.class_factory");


#endif // QUNKNWN_H
