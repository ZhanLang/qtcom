#ifndef _Q_I_PROPERYSET_H_
#define _Q_I_PROPERYSET_H_
#include "QIUnknown.h"

#include <QVariant>
struct QIPropertySet : public QIUnknown
{
    virtual QVariant getProperty(const QString& key) = 0;
    virtual QHRESULT addProperty(const QString& key, const QVariant& value) = 0;
    virtual QHRESULT addProperty(const QVariantMap& values) = 0;

    virtual bool isPropertyExist(const QString& key) = 0;

    virtual QStringList getKeys() = 0;
    virtual QVariantMap getProperties() = 0;

    virtual void clear() = 0;
};
QT_DEFINE_IID(QIPropertySet,"qtcom.i.propertyset");

QT_DEFINE_CLSID(CLSID_QPropertySet, "qtcom.class.propertyset")

#endif
