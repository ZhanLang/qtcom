#ifndef QPLUGINCONTAINER_H
#define QPLUGINCONTAINER_H

#include"qunknwnimpl.h"
#include"qcomptr.h"


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

    QSTDMETHOD(initPlugins)();
    QSTDMETHOD(unInitPlugins)();

    QSTDMETHOD(startPlugins)();
    QSTDMETHOD(stopPlugins)(int returnCode);

private:
    QtComPtr<QIRunningObjectTable> m_rot;


};

#endif // QPLUGINCONTAINER_H
