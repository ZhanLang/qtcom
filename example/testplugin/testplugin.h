#ifndef TESTPLUGIN_H
#define TESTPLUGIN_H

#include "testplugin_global.h"

#include<QIPlugin.h>
#include<QIUnknownImpl.h>
#include<QIRunningObjectTable.h>
#include<QtComPtr.h>
class  Testplugin : public QIPlugin , public QIPluginRun, public QIUnknownImpl
{
public:
    Testplugin();

    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIPlugin)
        QTCOM_QUERYINTERFACE_ENTRY(QIPluginRun)
    QTCOM_QUERYINTERFACE_END;

    virtual QHRESULT init_class(QIUnknown* rot, QIUnknown* outer);

    virtual QHRESULT Init(QIPropertySet* );
    virtual QHRESULT UnInit(int returnCode);

    virtual QHRESULT Start();
    virtual QHRESULT Stop();

private:
    QtComPtr<QIRunningObjectTable> m_rot;
};

#endif // TESTPLUGIN_H

QT_DEFINE_CLSID(CLSID_TestPlugin,"qtcom.plugin.testplugin")
