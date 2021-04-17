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
    m_clsContainer = new QClassContainer(nullptr);
}

QtComApplication::~QtComApplication()
{
    m_rot = QINull;
    m_prop = QINull;
}

QHRESULT QtComApplication::init_class(QIUnknown*, QIUnknown *)
{
    if( !m_clsContainer )
        return QE_RUNTIME;

    QRFAILED(m_clsContainer->CreateInstance( CLSID_QRunningObjectTable,
                                              qt_uuidof(QIRunningObjectTable),
                                              (void**)&m_rot.m_p));

    QRFAILED(m_rot->Register(CLSID_QClassContainer,
                             (QIUnknown*)((QIClassContainer*)m_clsContainer)));

    QRFAILED(m_clsContainer->CreateInstance( CLSID_QPropertySet,
                                             qt_uuidof(QIPropertySet),
                                             (void**)&m_prop.m_p));

    QRFAILED(m_clsContainer->CreateInstance( CLSID_QPluginContainer,
                                             qt_uuidof(QIPluginContainer),
                                            (void**)&m_pluginContainer.m_p, m_rot));

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

    return QS_OK;
}

QSTDMETHOD_IMPL QtComApplication::Exec(int argc, char *argv[])
{
    Q_UNUSED(argc) Q_UNUSED(argv)


    return QS_OK;
}

QSTDMETHOD_IMPL QtComApplication::Quit(int returnCode)
{
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
    for( const QString& key : o.keys())
    {
        m_prop->addProperty(key, o.value(key).toVariant());
    }
}

QHRESULT initModule(QJsonDocument &doc)
{
    return QS_OK;
}

QHRESULT initPlugin(QJsonDocument &doc)
{
return QS_OK;
}


