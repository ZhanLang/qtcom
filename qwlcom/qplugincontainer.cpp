#include "qplugincontainer.h"

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
    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::registerPluginsFile(const QString& cfgfile)
{
    return QS_OK;
}


QSTDMETHOD_IMPL QPluginContainer::registerPluginsFiles(const QStringList& files)
{
    return QS_OK;
}


QSTDMETHOD_IMPL QPluginContainer::initPlugins()
{
    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::unInitPlugins()
{
    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::startPlugins()
{
    return QS_OK;
}

QSTDMETHOD_IMPL QPluginContainer::stopPlugins(int returnCode)
{
    return QS_OK;
}
