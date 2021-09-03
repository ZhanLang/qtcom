#include "qtcomapplication.h"

#include"qrunningobjecttable.h"
#include<QPropertySet.h>
#include"qclasscontainer.h"


#include<QDomDocument>
#include<QFileInfo>
#include<QDebug>
#include<QFile>
#include<QDir>

extern "C" QHRESULT  DllGetClassObject( const QCLSID& clsid, const QIID& iid, void** pCls);
QtComApplication::QtComApplication( )
{
    m_cclass = new QClassContainer();
    m_plugins = new QPluginContainer();
    m_prop = new QPropertySet();
    m_rot = new QRunningObjectTableImpl();
}

QtComApplication::~QtComApplication()
{
    m_rot = QINull;
    m_prop = QINull;
    delete  m_plugins;
}

QHRESULT QtComApplication::init_class(QIUnknown*, QIUnknown *)
{
    if( !m_cclass )
        return QE_RUNTIME;

    m_plugins->init_class(m_rot,nullptr);
    m_rot->Register(CLSID_QClassContainer,(QIUnknown*)((QIClassContainer*)m_cclass));
    m_rot->Register(CLSID_QtApplaction, (QIUnknown*)((QIApplication*)this));
    return QS_OK;
}

QHRESULT QtComApplication::setConfigureFile(const QString& cfgfile)
{
    QFile jsonFile(cfgfile);
    if (!jsonFile.open(QFile::ReadOnly))
        return QE_INVALIDARG;

    return setConfigure(jsonFile.readAll());
}

QHRESULT QtComApplication::setConfigure(const QByteArray& cfg)
{
    QDomDocument doc;
    if( !doc.setContent( cfg ))
        return QE_INVALIDARG;

    //load prop
    QDomElement root = doc.firstChildElement();
    QDomElement propElement =  root.firstChildElement("properties");

    QDomElement prop = propElement.firstChildElement();
    for( ; !prop.isNull() ; prop = prop.nextSiblingElement())
    {
         QString key = prop.nodeName();
         QString value = prop.firstChild().nodeValue();

         m_prop->addProperty( key, value);
    }

    //init module

    QDomElement moduleElement = root.firstChildElement("module");
    QDomElement module = moduleElement.firstChildElement();
    for( ; !module.isNull() ; module = module.nextSiblingElement())
    {
         QHRESULT hr = addMoudle( module.nodeName(), module.firstChild().nodeValue());
         if( hr != QS_OK )
             return hr;
    }

    //init plugin
    QDomElement pluginModule = root.firstChildElement("plugin");
    QDomElement plugin = pluginModule.firstChildElement();
    for( ; !plugin.isNull() ; plugin = plugin.nextSiblingElement())
    {
         QHRESULT hr = addPlugin(plugin.nodeName(), plugin.firstChild().nodeValue());
         if( hr != QS_OK )
             return hr;
    }


    return QS_OK;
}

QHRESULT QtComApplication::Exec(int argc, char *argv[])
{
    Q_UNUSED(argc) Q_UNUSED(argv)

    QFileInfo fileInfo(QString::fromLocal8Bit(argv[0]));
    m_currentPath = fileInfo.absoluteFilePath();
    m_currentDir = fileInfo.absolutePath();


    QHRESULT hr = m_plugins->initPlugins();
    if( QFAILED(hr) )
    {
        return hr;
    }

    hr = m_plugins->startPlugins();
    if( QFAILED(hr))
    {
        return hr;
    }

    return QS_OK;
}

QHRESULT QtComApplication::Quit(int returnCode)
{
    if( m_plugins )
    {
        m_plugins->stopPlugins();
        m_plugins->unInitPlugins(returnCode);
    }

    if( m_rot )
    {
        m_rot->RevokeAll();
        m_rot = QINull;
    }

    if( m_cclass )
    {
        m_cclass->RevokeAll();
        m_cclass = QINull;
    }

    m_prop = QINull;
    return QS_OK;
}

QIPropertySet* QtComApplication::getPropertySet()
{
    return m_prop;
}

QIRunningObjectTable* QtComApplication::getRunningObjectTable()
{
    return m_rot.m_p;
}


QStringList QtComApplication::enumerate_xmlfile(const QString& path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.xml";

    QStringList files;
    QFileInfoList li = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    for (QFileInfo info : li)
    {
        files.append(info.absoluteFilePath());
    }
    return files;
}


QHRESULT QtComApplication::addMoudle(const QString &name, const QString &path)
{
    QStringList files = enumerate_xmlfile( path );
    if( files.isEmpty() )
    {
        qDebug() << "module configure path: " << path << " not find xml files. " << name;
        return QS_OK;
    }

    QClassContainer* module = static_cast<QClassContainer*>(m_cclass.m_p);
    return module->registerModulesFiles(files);
}

QHRESULT QtComApplication::addPlugin(const QString &name, const QString &path)
{
    QStringList files = enumerate_xmlfile( path );
    if( files.isEmpty() )
    {
        qDebug() << "plugin configure path: " << path << " not find xml files. " << name;
        return QS_OK;
    }

    return m_plugins->registerPluginsFiles(files);
}

QString QtComApplication::getCurrentPath()
{
    return m_currentPath;
}

QString QtComApplication::getCurrentDir()
{
    return m_currentDir;
}
