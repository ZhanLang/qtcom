#ifndef _Q_I_CLASS_CONTAINER_H_
#define _Q_I_CLASS_CONTAINER_H_

#include"QIUnknown.h"

struct QIClassContainer : public QIUnknown
{
    QSTDMETHOD(CreateInstance)(const QCLSID& clsid,const QIID& iid, void **ppv, QIUnknown *prot = QINull, QIUnknown *pUnkOuter = QINull) = 0;

    QSTDMETHOD(GetClassObject)(const QCLSID& clsid,const QIID& factoryiid, void **pCls) = 0;

    QSTDMETHOD(Register)(const QCLSID& clsid, const QString& path, const QString& libName) = 0;

    QSTDMETHOD_(bool, isRegistered)(const QCLSID& clsid) = 0;

    QSTDMETHOD(Revoke)(const QCLSID& clsid) = 0;

    QSTDMETHOD_(void,RevokeAll)() = 0;
};
QT_DEFINE_IID(QIClassContainer,"qtcom.i.classcontainer");
QT_DEFINE_CLSID(CLSID_QClassContainer,"qtcom.class.classcontainer");
#endif
