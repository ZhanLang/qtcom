#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>


#include "qclasscontainer.h"


QClassContainer::QClassContainer(void* parent)
{
    Q_UNUSED(parent)
}

QHRESULT QClassContainer::init_class( QIUnknown*, QIUnknown*)
{
    return QS_OK;
}

QClassContainer::~QClassContainer()
{
    RevokeAll();
}


QSTDMETHOD_IMPL QClassContainer::CreateInstance(const QCLSID& clsid,const QIID& iid, void **ppv, QIUnknown *prot,void* parent, QIUnknown *pUnkOuter)
{
    QtComPtr<QIClassFactory> pCls;
    QRFAILED(GetClassObject(clsid, qt_uuidof(QIClassFactory), (void**)&pCls.m_p));

    return pCls->CreateInstance(iid, ppv, prot, parent, pUnkOuter);
}

extern "C" QHRESULT  QtDllGetClassObject( const QCLSID& clsid, const QIID& iid, void** pCls);

typedef int (*QtDllGetClassObjectFunc)(const QCLSID& , const QIID& , void** );

QSTDMETHOD_IMPL QClassContainer::GetClassObject(const QCLSID& clsid, const QIID& iid,void **ppv)
{
    if( clsid == CLSID_QPluginContainer     ||
        clsid == CLSID_QRunningObjectTable   ||
        clsid == CLSID_QPropertySet         ||
        clsid == CLSID_QtApplaction
      )
    {
        return QtDllGetClassObject(clsid, iid, ppv);
    }


    QMutexLocker locker(&m_mutex);
    if( !m_clsobjs.contains(clsid))
        return QE_NOTFIND;

    QString libfile = m_clsobjs[clsid];
    QtDllGetClassObjectFunc func = (QtDllGetClassObjectFunc)QLibrary::resolve(libfile, "QtDllGetClassObject");
    if (!func)
    {
        qCritical() << "get library function QtDllGetClassObject faild :" << libfile;
        return QE_NOTFIND;
    }

    return func(clsid, iid, ppv);
}

QSTDMETHOD_IMPL QClassContainer::Register(const QCLSID& clsid, const QString& path, const QString& libName)
{
    QMutexLocker locker(&m_mutex);
    if( m_clsobjs.contains(clsid))
        return QS_OK;

    QString libPath = path +  libName ;
    m_clsobjs.insert(clsid, libPath);
    return QS_OK;
}

QSTDMETHOD_IMPL QClassContainer::registerModules(const QByteArray& cfg)
{
    QJsonDocument jsoncfg = QJsonDocument::fromJson(cfg);

    if(!jsoncfg.isArray())
        return QE_INVALIDARG;

    for( QJsonValue v : jsoncfg.array())
    {
        QJsonObject o = v.toObject();
        QString libpath = o.value("libpath").toString();
        QString libname = o.value("libname").toString();

        for( QJsonValue vc : o.value("class").toArray())
        {
            QJsonObject oc = vc.toObject();
            QString clsid = oc.value("clsid").toString();
            QHRESULT hr =  Register( clsid, libpath, libname);
            if( QFAILED(hr))
                return hr;
        }
    }

    return QS_OK;
}

QSTDMETHOD_IMPL QClassContainer::registerModulesFile(const QString& cfgfile)
{
    QFile jsonFile(cfgfile);
    if(!jsonFile.open(QFile::ReadOnly))
        return QE_INVALIDARG;

    return registerModules( jsonFile.readAll() );
}

QSTDMETHOD_IMPL QClassContainer::registerModulesFiles(const QStringList& files)
{
    for (const QString& file : files)
    {
        QHRESULT hResult = registerModulesFile(file);
        if (QFAILED(hResult))
        {
            //todo:
            return hResult;
        }
    }

    return QS_OK;
}

QSTDMETHOD_IMPL_(bool)QClassContainer::isRegistered(const QCLSID& clsid)
{
    QMutexLocker locker(&m_mutex);
    return m_clsobjs.contains(clsid);
}

QSTDMETHOD_IMPL QClassContainer::Revoke(const QCLSID& clsid)
{
    QMutexLocker locker(&m_mutex);
    if( !m_clsobjs.contains(clsid))
        return QE_NOTFIND;

    m_clsobjs.remove(clsid);
    return QS_OK;
}

QSTDMETHOD_IMPL_(void) QClassContainer::RevokeAll()
{
    m_clsobjs.clear();
}
