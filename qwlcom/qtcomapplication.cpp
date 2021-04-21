#include "qtcomapplication.h"

#include"qrunningobjecttable.h"
#include"qpropertyset.h"
#include"qclasscontainer.h"
#include<QJsonObject>
#include<QJsonArray>
#include<QFile>
#include<QFileInfo>
#include<QDir>

extern "C" QHRESULT  DllGetClassObject( const QCLSID& clsid, const QIID& iid, void** pCls);
QtComApplication::QtComApplication(void * )
{
    m_cclass = new QClassContainer(nullptr);
}

QtComApplication::~QtComApplication()
{
    m_rot = QINull;
    m_prop = QINull;
}

QHRESULT QtComApplication::init_class(QIUnknown*, QIUnknown *)
{
    if( !m_cclass )
        return QE_RUNTIME;

    QRFAILED(m_cclass->CreateInstance( CLSID_QRunningObjectTable,
                                       qt_uuidof(QIRunningObjectTable),
                                       (void**)&m_rot.m_p));

    QRFAILED(m_rot->Register(CLSID_QClassContainer,
                             (QIUnknown*)((QIClassContainer*)m_cclass)));

    QRFAILED(m_cclass->CreateInstance( CLSID_QPropertySet,
                                       qt_uuidof(QIPropertySet),
                                       (void**)&m_prop.m_p));

    QRFAILED(m_cclass->CreateInstance( CLSID_QPluginContainer,
                                       qt_uuidof(QIPluginContainer),
                                       (void**)&m_plugins.m_p, m_rot));

    QRFAILED(m_rot->Register(CLSID_QtApplaction, (QIUnknown*)((QIApplication*)this)));

    return QS_OK;
}

QSTDMETHOD_IMPL QtComApplication::setConfigureFile(const QString& cfgfile)
{
    QFile jsonFile(cfgfile);
    if (!jsonFile.open(QFile::ReadOnly))
        return QE_INVALIDARG;

    return setConfigure(jsonFile.readAll());
}

QSTDMETHOD_IMPL QtComApplication::setConfigure(const QByteArray& cfg)
{
    QJsonDocument jsoncfg = QJsonDocument::fromJson(cfg);

    if (!jsoncfg.isObject())
        return QE_INVALIDARG;

    initPropterty(jsoncfg);

    QHRESULT hr = initModules(jsoncfg);
    if( QFAILED(hr) )
    {
        return hr;
    }

    hr = initPlugins(jsoncfg);
    if( QFAILED(hr))
    {
        return hr;
    }

    return QS_OK;
}

QSTDMETHOD_IMPL QtComApplication::Exec(int argc, char *argv[])
{
    Q_UNUSED(argc) Q_UNUSED(argv)

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

QSTDMETHOD_IMPL QtComApplication::Quit(int returnCode)
{
    if( m_plugins )
    {
        m_plugins->stopPlugins();
        m_plugins->unInitPlugins(returnCode);
        m_plugins = QINull;
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

QSTDMETHOD_IMPL_ (QIPropertySet*) QtComApplication::getPropertySet()
{
    return m_prop;
}

QSTDMETHOD_IMPL_(QIRunningObjectTable*) QtComApplication::getRunningObjectTable()
{
    return m_rot.m_p;
}


void QtComApplication::initPropterty(QJsonDocument &doc)
{
    QJsonValue v = doc.object().value("properties");
    if( !v.isObject() )
        return;

    QJsonObject o = v.toObject();
    m_prop->addProperty(o.toVariantMap());
}

QHRESULT QtComApplication::initModules(QJsonDocument& doc)
{
    QJsonObject o = doc.object();
    for (QJsonValue v : o.value("module.files").toArray())
    {
        QString file = v.toString();
        QHRESULT hr = m_cclass->registerModulesFile(file);
        if (QFAILED(hr))
        {
            return hr;
        }
    }

    for (QJsonValue v : o.value("module.paths").toArray())
    {
        QStringList files = enumerate_jsonfile(v.toString());
        QHRESULT hr = m_cclass->registerModulesFiles(files);
        if (QFAILED(hr))
        {
            return hr;
        }
    }

    return QS_OK;
}

QHRESULT QtComApplication::initPlugins(QJsonDocument& doc)
{
    QJsonObject o = doc.object();
    for (QJsonValue v : o.value("plugin.files").toArray())
    {
        QString file = v.toString();
        QHRESULT hr = m_plugins->registerPluginsFile(file);
        if (QFAILED(hr))
        {
            return hr;
        }
    }

    for (QJsonValue v : o.value("plugin.files").toArray())
    {
        QStringList files = enumerate_jsonfile(v.toString());
        QHRESULT hr = m_plugins->registerPluginsFiles(files);
        if (QFAILED(hr))
        {
            return hr;
        }
    }
    return QS_OK;
}

QStringList QtComApplication::enumerate_jsonfile(const QString& path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.json";

    QStringList files;
    QFileInfoList li = dir.entryInfoList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    for (QFileInfo info : li)
    {
        files.append(info.absoluteFilePath());
    }
    return files;
}

