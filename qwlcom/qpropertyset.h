#ifndef QPROPERTYSET_H
#define QPROPERTYSET_H
#include"qunknwnimpl.h"
#include<QMap>
#include<QMutex>
class QPropertySet : public QIPropertySet, public QUnknownImp
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIPropertySet)
        QTCOM_QUERYINTERFACE_ENTRY(QIPropertySet)
    QTCOM_QUERYINTERFACE_END
    virtual QHRESULT init_class( QIUnknown* , QIUnknown*)
    {
        return QS_OK;
    }
    QPropertySet(void*)
    {

    }
protected:
    QSTDMETHOD_(QVariant,getProperty)(const QString& key)
    {
        QMutexLocker locker(&m_mutex);
        if( m_props.contains(key))
            return m_props[key];

        return QVariant();
    }

    QSTDMETHOD(addProperty)(const QString& key, const QVariant& value)
    {
        QMutexLocker locker(&m_mutex);
         m_props.insert(key, value);
         return QS_OK;
    }

    QSTDMETHOD(addProperty)(const QMap<QString, QVariant>& values)
    {
        QMap<QString, QVariant>::ConstIterator it = values.constBegin();
        for( ; it != values.constEnd(); it++)
        {
            addProperty(it.key(),it.value());
        }
         return QS_OK;
    }

    QSTDMETHOD_(bool,isPropertyExist)(const QString& key)
    {
        QMutexLocker locker(&m_mutex);
        return m_props.contains(key);
    }

    QSTDMETHOD_(QStringList,getPropertyKeys)()
    {
        QMutexLocker locker(&m_mutex);
        return m_props.keys();
    }

private:
    QMutex m_mutex;
    QMap<QString, QVariant> m_props;
};

#endif // QPROPERTYSET_H
