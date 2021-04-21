#ifndef QPLUGINCONTAINER_H
#define QPLUGINCONTAINER_H

#include"qunknwnimpl.h"
#include"qcomptr.h"

class QtComPlugin
{
public:
    int index;
    QString clsid;
    QMap<QString, QVariant> properties;
};

class QPluginContainer : public QIPluginContainer , public QUnknownImp
{

public:
    QPluginContainer(void*);
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIPluginContainer)
        QTCOM_QUERYINTERFACE_ENTRY(QIPluginContainer)
    QTCOM_QUERYINTERFACE_END

    QHRESULT init_class( QIUnknown* rot, QIUnknown*);

protected:
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
