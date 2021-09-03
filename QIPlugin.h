#ifndef _Q_I_PLUGIN_H_
#define _Q_I_PLUGIN_H_

#include"QIUnknown.h"
#include "QIPropertySet.h"

struct QIPlugin : public QIUnknown
{
    virtual QHRESULT Init(QIPropertySet* ) = 0;
    virtual QHRESULT UnInit(int returnCode) = 0;
};
QT_DEFINE_IID(QIPlugin,"qtcom.i.plugin");

struct QIPluginRun : public QIUnknown
{
    virtual QHRESULT Start() = 0;
    virtual QHRESULT Stop() = 0;
};
QT_DEFINE_IID(QIPluginRun,"qtcom.i.plugin_run");

#endif
