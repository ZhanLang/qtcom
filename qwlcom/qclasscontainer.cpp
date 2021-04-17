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

extern "C" QHRESULT  DllGetClassObject( const QCLSID& clsid, const QIID& iid, void** pCls);
QSTDMETHOD_IMPL QClassContainer::GetClassObject(const QCLSID& clsid, const QIID& iid,void **ppv)
{
    if( clsid == CLSID_QPluginContainer     ||
        clsid == CLSID_QRunningObjectTable   ||
        clsid == CLSID_QPropertySet         ||
        clsid == CLSID_QtApplaction
      )
    {
        return DllGetClassObject(clsid, iid, ppv);
    }

    QMutexLocker locker(&m_mutex);
    if( !m_clsobjs.contains(clsid))
        return QE_NOTFIND;

    QComLibrary* lib = m_clsobjs[clsid];
    if( !lib )
        return QE_RUNTIME;

    if( !lib->isLoaded() && !lib->open())
    {
         //todo log
         return QE_UNEXPECTED;
    }

    return lib->DllGetClassObject(clsid, iid, ppv);
}

QSTDMETHOD_IMPL QClassContainer::Register(const QCLSID& clsid, const QString& path, const QString& libName)
{
    QMutexLocker locker(&m_mutex);
    if( m_clsobjs.contains(clsid))
        return QE_EXIST;


#ifdef Q_OS_WIN
    QString libPath = path + libName + ".dll";
#elif
    QString libPath = path +"lib"+ libName + ".so";
#endif
    if(!QLibrary::isLibrary(libPath))
    {
        //todo log
        return QE_UNEXPECTED;
    }

    QComLibrary* lib = new QComLibrary();
    if( !lib )
        return QE_UNEXPECTED;

    lib->setFileName(path);
    m_clsobjs.insert(clsid, lib);

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
    if(jsonFile.open(QFile::ReadOnly))
        return QE_INVALIDARG;

    return registerModules( jsonFile.readAll() );
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

    QComLibrary* lib = m_clsobjs[clsid];
    if( lib )
        delete lib;

    m_clsobjs.remove(clsid);
    return QS_OK;
}

QSTDMETHOD_IMPL_(void) QClassContainer::RevokeAll()
{
    QMutexLocker locker(&m_mutex);
    QHash<QCLSID, QComLibrary*>::iterator i = m_clsobjs.begin();
    for (; i != m_clsobjs.end(); ++i)
    {
        QComLibrary* lib = i.value();
        if( lib )
            delete lib;
    }

    m_clsobjs.clear();
}
