#ifndef QPLUGINCONTAINER_H
#define QPLUGINCONTAINER_H

#include<QIUnknown.h>
#include<QString>
#include<QMap>
#include<QtComPtr.h>
#include<QIRunningObjectTable.h>

#include <QVariant>
class QtComPlugin
{
public:
    int index;
    QCLSID clsid;
    QtComPtr<QIUnknown> obj;
    QMap<QString, QVariant> properties;
};

class QPluginContainer
{

public:
    QPluginContainer();
    virtual ~QPluginContainer();
    QHRESULT init_class( QIUnknown* rot , QIUnknown* );

public:
    virtual QHRESULT Register(const QCLSID& clsid, int index, const QMap<QString, QVariant>& prop);

    QSTDMETHOD(registerPlugins)(const QByteArray& cfg);
    QSTDMETHOD(registerPluginsFile)(const QString& cfgfile);
    QSTDMETHOD(registerPluginsFiles)(const QStringList& cfgfile);

    QSTDMETHOD_(bool, isRegistered)(const QCLSID& clsid);

    QSTDMETHOD(initPlugins)();
    QSTDMETHOD(unInitPlugins)(int returnCode);

    QSTDMETHOD(startPlugins)();
    QSTDMETHOD(stopPlugins)();


private:
    QtComPtr<QIRunningObjectTable> m_rot;
    QList<QtComPlugin> m_plugins;
};

#endif // QPLUGINCONTAINER_H
