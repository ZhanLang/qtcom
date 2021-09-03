#include "qplugincontainer.h"

#include<QFile>
#include<QDomDocument>
#include<QDebug>
#include<QPropertySet.h>
#include<QIPlugin.h>

bool lessThanPlugin(const QtComPlugin &v1, const QtComPlugin &v2)
{
    return v1.index < v2.index;
}


QPluginContainer::QPluginContainer()
{

}

QPluginContainer::~QPluginContainer(){

}

QHRESULT QPluginContainer::init_class(QIUnknown *rot, QIUnknown *)
{
    QRASSERT(rot, QE_NOROT);
    m_rot = rot;

    return QS_OK;
}


QHRESULT QPluginContainer::Register(const QCLSID& clsid, int index, const QMap<QString, QVariant>& prop)
{
    if( m_rot->isRunning(clsid))
        return QS_OK;

    QtComPtr<QIUnknown> p;
    QHRESULT hr = m_rot->CreateInstance(clsid, qt_uuidof(QIUnknown), (void**)&p.m_p);
    if( hr != QS_OK )
    {
        qCritical() << "QPluginContainer Register object " << clsid << " create faild.";
        return hr;
    }

    m_plugins.append({ index,clsid, p, prop});
    hr = m_rot->Register( clsid, p);
    if( hr != QS_OK )
    {
        qCritical() << "QPluginContainer Register to rot faild " << clsid;
        return hr;
    }

    qSort(m_plugins.begin(), m_plugins.end(), lessThanPlugin);

    return QS_OK;
}
QSTDMETHOD_IMPL QPluginContainer::registerPlugins(const QByteArray& cfg)
{
    QDomDocument doc;
    if(! doc.setContent( cfg ))
        return QE_INVALIDARG;


    QDomElement root = doc.firstChildElement("plugin");
    QDomElement classElement = root.firstChildElement("class");

    QDomElement clsid = classElement.firstChildElement();
    for( ; !clsid.isNull() ; clsid = clsid.nextSiblingElement())
    {
        QString id = clsid.attribute("id");
        QString index = clsid.attribute("index");

        QVariantMap propMap;

        QDomElement prop = clsid.firstChildElement("properties").firstChildElement();
        for( ; !prop.isNull() ; prop = prop.nextSiblingElement())
        {
            propMap.insert( prop.nodeName(), prop.firstChild().nodeValue());
        }

        QHRESULT hr = Register( id, index.toInt(), propMap);
        if( hr != QS_OK )
            return hr;
    }


    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::registerPluginsFile(const QString& cfgfile)
{
    QFile jsonFile(cfgfile);
    if(!jsonFile.open(QFile::ReadOnly))
        return QE_INVALIDARG;

    return registerPlugins( jsonFile.readAll() );
}


QSTDMETHOD_IMPL QPluginContainer::registerPluginsFiles(const QStringList& files)
{
    for( const QString& file : files)
    {
        QHRESULT hr = registerPluginsFile(file);
        if( hr != QS_OK )
        {
            qCritical() << "QPluginContainer registerPluginsFiles faild: " << file << " result:" << hr;
            return hr;
        }
    }

    return QS_OK;
}

QSTDMETHOD_IMPL_(bool) QPluginContainer::isRegistered(const QCLSID& clsid)
{
    QList<QtComPlugin>::iterator it = m_plugins.begin();
    for( ; it != m_plugins.end(); it++)
    {
        if( it->clsid == clsid)
            return true;
    }

    return false;
}

QSTDMETHOD_IMPL QPluginContainer::initPlugins()
{
    QList<QtComPlugin>::iterator it = m_plugins.begin();
    for( ; it!= m_plugins.end(); it++)
    {
        const QtComPlugin& plugin = *it;
        QtComPtr<QIPlugin> p = plugin.obj;
        if( !p )
            continue;

        QtComPtr<QIPropertySet> pset = new QPropertySet();
        pset->addProperty(plugin.properties);

        QHRESULT hr = p->Init( pset );
        if( hr != QS_OK )
        {
            qCritical() << "init plugin faild: " << plugin.clsid << " result:" << hr;
            return hr;
        }

        qDebug() << "init plugin:" << plugin.clsid;
    }

    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::unInitPlugins(int returnCode)
{
    QList<QtComPlugin>::reverse_iterator it = m_plugins.rbegin();
    for( ; it!= m_plugins.rend(); it++)
    {
        const QtComPlugin& plugin = *it;
        QtComPtr<QIPlugin> p = plugin.obj;
        if( !p )
            continue;

        QHRESULT hr = p->UnInit( returnCode);
        if( hr != QS_OK)
        {
            qWarning() << "uninit plugin faild: " << plugin.clsid << " result:" << hr;
        }
        else
        {
            qDebug() << "uninit plugin:" << plugin.clsid;
        }
    }

    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::startPlugins()
{
    QList<QtComPlugin>::iterator it = m_plugins.begin();
    for( ; it!= m_plugins.end(); it++)
    {
        const QtComPlugin& plugin = *it;
        QtComPtr<QIPluginRun> p = plugin.obj;
        if( !p )
            continue;

        QHRESULT hr = p->Start();
        if( hr != QS_OK)
        {
            qWarning() << "QPluginContainer start plugin faild: " << plugin.clsid << " result:" << hr;
            return hr;
        }

        qDebug() << "start plugin " << plugin.clsid;
    }

    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::stopPlugins()
{
    QList<QtComPlugin>::reverse_iterator it = m_plugins.rbegin();
    for( ; it!= m_plugins.rend(); it++)
    {
        const QtComPlugin& plugin = *it;
        QtComPtr<QIPluginRun> p = plugin.obj;
        if( !p )
            continue;

        QHRESULT hr = p->Stop();
        if( hr != QS_OK)
        {
            qWarning() << "stop plugin faild: " << plugin.clsid << " result:" << hr;
        }
        else
        {
            qDebug() << "stop plugin:" << plugin.clsid;
        }
    }

    return QS_OK;
}
