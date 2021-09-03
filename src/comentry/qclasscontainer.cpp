#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>


#include "qclasscontainer.h"

#include<QIClassFactory.h>
#include<QIApplication.h>
#include<QFileInfo>

#include<QCoreApplication>
#include<QDir>

#include<QDomDocument>

QClassContainer::QClassContainer()
{

}


QHRESULT QClassContainer::init_class( QIUnknown*, QIUnknown*)
{
    return QS_OK;
}

QClassContainer::~QClassContainer()
{

}


QSTDMETHOD_IMPL QClassContainer::CreateInstance(const QCLSID& clsid,const QIID& iid, void **ppv, QIUnknown *prot, QIUnknown *pUnkOuter)
{
    QtComPtr<QIClassFactory> pCls;
    QRFAILED(GetClassObject(clsid, qt_uuidof(QIClassFactory), (void**)&pCls.m_p));

    return pCls->CreateInstance(iid, ppv, prot, pUnkOuter);
}

extern "C" QHRESULT  QtDllGetClassObject( const QCLSID& clsid, const QIID& iid, void** pCls);

typedef int (*QtDllGetClassObjectFunc)(const QCLSID& , const QIID& , void** );

QSTDMETHOD_IMPL QClassContainer::GetClassObject(const QCLSID& clsid, const QIID& iid,void **ppv)
{
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

    QHRESULT hr = func(clsid, iid, ppv);
    if( hr != QS_OK )
    {
        qWarning() << "create object faild:" << clsid << " in: " << libfile;
    }

    return hr;
}

QSTDMETHOD_IMPL QClassContainer::Register(const QCLSID& clsid, const QString& path, const QString& libName)
{
    QMutexLocker locker(&m_mutex);
    if( m_clsobjs.contains(clsid))
        return QS_OK;

    QString libPath = QCoreApplication::applicationDirPath() + "/" + path +  libName ;
    m_clsobjs.insert(clsid, libPath);
    return QS_OK;
}

QSTDMETHOD_IMPL QClassContainer::registerModules(const QByteArray& cfg)
{
    QDomDocument doc;
    if( !doc.setContent( cfg ))
        return QE_INVALIDARG;


    QDomElement moduleElement = doc.firstChildElement("module");
    QString libpath = moduleElement.firstChildElement("path").firstChild().nodeValue();
    QString libname = moduleElement.firstChildElement("name").firstChild().nodeValue();
    QDomElement classElement = moduleElement.firstChildElement("class");

    QDomNode clsidNode = classElement.firstChildElement();
    for( ; !clsidNode.isNull() ; clsidNode = clsidNode.nextSiblingElement())
    {
        QString clsid = clsidNode.firstChild().nodeValue();
        QHRESULT hr =  Register( clsid, libpath, libname);
        if( QFAILED(hr))
            return hr;
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
            qDebug() << "regedit module configure file failed " << file;
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
