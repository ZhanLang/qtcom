#ifndef _Q_I_APPLICATION_H_
#define _Q_I_APPLICATION_H_
#include"QIUnknown.h"
#include"QIPropertySet.h"
#include"QIRunningObjectTable.h"

struct QIApplication : public QIUnknown
{
    //default configure file is app.config.xml
    virtual QHRESULT setConfigureFile(const QString& cfgfile) = 0;

    virtual QHRESULT Exec(int argc, char *argv[]) = 0;

    virtual QHRESULT Quit(int returnCode) = 0;

    virtual QIPropertySet* getPropertySet() = 0;

    virtual QIRunningObjectTable* getRunningObjectTable() = 0;

    virtual QString getCurrentPath() = 0;
    virtual QString getCurrentDir() = 0;
};
QT_DEFINE_IID(QIApplication,"qtcom.i.applaction");
QT_DEFINE_CLSID(CLSID_QtApplaction, "qtcom.class.applaction");

#endif
