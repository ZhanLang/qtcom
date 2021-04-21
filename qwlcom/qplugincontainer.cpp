#include "qplugincontainer.h"

#include<QFile>
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QDebug>
#include"qpropertyset.h"

bool lessThanPlugin(const QtComPlugin &v1, const QtComPlugin &v2)
{
    return v1.index < v2.index;
}


QPluginContainer::QPluginContainer(void*)
{

}

QHRESULT QPluginContainer::init_class(QIUnknown *rot, QIUnknown *)
{
    QRASSERT(rot, QE_NOROT);
    m_rot = rot;

    return QS_OK;
}


QSTDMETHOD_IMPL QPluginContainer::registerPlugins(const QByteArray& cfg)
{
    QJsonDocument jsoncfg = QJsonDocument::fromJson(cfg);
    if(!jsoncfg.isArray())
        return QE_INVALIDARG;

    for( QJsonValue v : jsoncfg.array())
    {
        QJsonObject o = v.toObject();

        QString clsid = o.value("clsid").toString();
        if( isRegistered(clsid))
            continue;

        QtComPlugin plugin;
        plugin.clsid = clsid;
        plugin.index = o.value("index").toInt();

        QJsonObject properties = o.value("properties").toObject();
        plugin.properties = properties.toVariantMap();

        m_plugins.append(plugin);
    }

    qSort(m_plugins.begin(), m_plugins.end(), lessThanPlugin);
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
        QRFAILED(registerPluginsFile(file));
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
    for(; it!= m_plugins.end() ; it++ )
    {
        if( m_rot->isRunning(it->clsid))
        {
            qInfo() << "QPluginContainer plugin is running:" << it->clsid;
            continue;
        }

        QtComPtr<QIUnknown> p;
        QHRESULT hr = m_rot->CreateInstance(it->clsid, qt_uuidof(QIUnknown), (void**)&p.m_p);
        if( QFAILED( hr ) )
        {
            qCritical() << "QPluginContainer CreateInstance: " << it->clsid << " result:" << hr;
            return hr;
        }

        QtComPtr<QIPlugin> plugin = p;
        if( plugin )
        {
            QtComPtr<QIPropertySet> pset = new QPropertySet(nullptr);
            pset->addProperty(it->properties);

            hr = plugin->Init( pset );
            if( QFAILED( hr ))
            {
                qCritical() << "QPluginContainer init plugin faild: " << it->clsid << " result:" << hr;
                return hr;
            }
        }

        m_rot->Register( it->clsid, p);
    }
    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::unInitPlugins(int returnCode)
{
    QListIterator<QtComPlugin> iterator(m_plugins);
    while(iterator.hasPrevious())
    {
        const QtComPlugin& plugin = iterator.previous();
        QtComPtr<QIPlugin> p;
        m_rot->GetObject( plugin.clsid, qt_uuidof(QIPlugin), (void**)&p.m_p);
        if( p )
        {
            QHRESULT hr = p->UnInit( returnCode);
            if(QFAILED(hr))
            {
                qWarning() << "QPluginContainer uninit plugin faild: " << plugin.clsid << " result:" << hr;
            }
        }
    }

    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::startPlugins()
{
    QListIterator<QtComPlugin> iterator(m_plugins);
    while(iterator.hasNext())
    {
        const QtComPlugin& plugin = iterator.next();
        QtComPtr<QIPluginRun> p;
        m_rot->GetObject( plugin.clsid, qt_uuidof(QIPluginRun), (void**)&p.m_p);
        if( p )
        {
            QHRESULT hr = p->Start();
            if(QFAILED(hr))
            {
                qWarning() << "QPluginContainer start plugin faild: " << plugin.clsid << " result:" << hr;
            }
        }
    }

    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::stopPlugins()
{
    QListIterator<QtComPlugin> iterator(m_plugins);
    while(iterator.hasPrevious())
    {
        const QtComPlugin& plugin = iterator.previous();
        QtComPtr<QIPluginRun> p;
        m_rot->GetObject( plugin.clsid, qt_uuidof(QIPluginRun), (void**)&p.m_p);
        if( p )
        {
            QHRESULT hr = p->Stop();
            if(QFAILED(hr))
            {
                qWarning() << "QPluginContainer stop plugin faild: " << plugin.clsid << " result:" << hr;
            }
        }
    }

    return QS_OK;
}
