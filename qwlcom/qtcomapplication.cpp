#include "qtcomapplication.h"

#include"qrunningobjecttable.h"
#include"qpropertyset.h"
#include"qclasscontainer.h"
#include<QJsonObject>
#include<QJsonArray>
#include<QFile>
#include<QFileInfo>
#include<QDir>

QtComApplication::QtComApplication(void * )
{
    m_rot = new QRunningObjectTableImpl(nullptr);
    m_prop = new QPropertySet(nullptr);
    m_clsContainer = new QClassContainer(nullptr);

    m_rot->Register(CLSID_QtApplaction, (QIUnknown*)((QIApplication*)this));
    m_rot->Register(CLSID_QtApplaction, (QIUnknown*)((QIClassContainer*)m_clsContainer));
}

QtComApplication::~QtComApplication()
{
    m_rot = QINull;
    m_prop = QINull;
}

QSTDMETHOD_IMPL QtComApplication::Exec(const QString& cfgFile)
{
    QRFAILED(load(cfgFile));

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
        m_prop->SetProperty(key, o.value(key).toVariant());
    }
}

QHRESULT QtComApplication::load(const QString &cfgFile)
{
    QFile jsonFile(cfgFile);
    if(jsonFile.open(QFile::ReadOnly))
        return QE_INVALIDARG;

    QJsonDocument jsoncfg = QJsonDocument::fromJson(jsonFile.readAll());
    if( !jsoncfg.isObject())
        return QE_INVALIDARG;

    initPropterty( jsoncfg);

    return QS_OK;
}

QHRESULT QtComApplication::loadModule(const QString& file)
{
    QFile jsonFile(file);
    if(jsonFile.open(QFile::ReadOnly))
        return QE_INVALIDARG;

    QJsonDocument jsoncfg = QJsonDocument::fromJson(jsonFile.readAll());
    if( !jsoncfg.isObject())
        return QE_INVALIDARG;

    if(!jsoncfg.isArray())
        return QE_INVALIDARG;

    QFileInfo info(file);
    QString path = info.path() + QDir::separator();
    for( QJsonValue v : jsoncfg.array())
    {
        QJsonObject o = v.toObject();
        QRFAILED(loadClass(path,o));
    }
}

QHRESULT QtComApplication::loadClass(const QString& cfgPath, QJsonObject &obj)
{
    QString libpath = obj.value("libpath").toString();
    QString libname = obj.value("libname").toString();

    if(libname.startsWith('.'))
        libname = cfgPath + libname;

    for( QJsonValue v : obj.value("class").toArray())
    {
        QJsonObject o = v.toObject();
        QHRESULT hr = m_clsContainer->Register(o.value("clsid").toString(), libpath, libname);
        if( QFAILED(hr))
        {
            //todo log
            return hr;
        }
    }

    return QS_OK;
}
