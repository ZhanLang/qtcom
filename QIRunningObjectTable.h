#ifndef _Q_I_RUNNING_OBJECT_TABLE_H_
#define _Q_I_RUNNING_OBJECT_TABLE_H_

#include"QIUnknown.h"

struct QIRunningObjectTable : public QIUnknown
{
    QSTDMETHOD(CreateInstance)(const QCLSID& clsid,const QIID& iid, void **ppv, QIUnknown *pUnkOuter = QINull) = 0;

    QSTDMETHOD(GetObject)(const QCLSID& clsid, const QIID& iid, void **ppunk) = 0;

    QSTDMETHOD(Register)(const QCLSID& clsid, QIUnknown* punk) = 0;
    QSTDMETHOD_(bool, isRunning)(const QCLSID& clsid) = 0;

    QSTDMETHOD(Revoke)(const QCLSID& clsid) = 0;
    QSTDMETHOD(RevokeAll)() = 0;
};
QT_DEFINE_IID(QIRunningObjectTable,"qtcom.i.running_object_table");

QT_DEFINE_CLSID(CLSID_QRunningObjectTable,"qcom.class.running_object_table");

#endif
