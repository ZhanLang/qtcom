#ifndef _Q_I_OBJECT_H_
#define _Q_I_OBJECT_H_

#include"QIUnknown.h"
struct QIQObject : public QIUnknown
{
    virtual QObject* getQObject() = 0;
};
QT_DEFINE_IID(QIQObject, "qtcom.i.combase.object");

#define QTCOM_DEFINE_QIQOBJECT \
    virtual QObject *getQObject(){return static_cast<QObject*>(this);}
#endif
