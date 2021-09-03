#ifndef QPROPERTYSET_H
#define QPROPERTYSET_H
#include<QIUnknownImpl.h>
#include<QIPropertySet.h>

#include<QMap>
#include<QMutex>
class QPropertySet : public QIPropertySet, public QIUnknownImpl
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIPropertySet)
        QTCOM_QUERYINTERFACE_ENTRY(QIPropertySet)
    QTCOM_QUERYINTERFACE_END

    QPropertySet()
    {

    }
protected:
    virtual QVariant getProperty(const QString& key)
    {
        QMutexLocker locker(&m_mutex);
        if( m_props.contains(key))
            return m_props[key];

        return QVariant();
    }

    virtual QHRESULT addProperty(const QString& key, const QVariant& value)
    {
        QMutexLocker locker(&m_mutex);
         m_props.insert(key, value);
         return QS_OK;
    }

    virtual QHRESULT addProperty(const QMap<QString, QVariant>& values)
    {
        QMap<QString, QVariant>::ConstIterator it = values.constBegin();
        for( ; it != values.constEnd(); it++)
        {
            addProperty(it.key(),it.value());
        }
         return QS_OK;
    }

    virtual bool isPropertyExist(const QString& key)
    {
        QMutexLocker locker(&m_mutex);
        return m_props.contains(key);
    }

    virtual QStringList getKeys()
    {
        QMutexLocker locker(&m_mutex);
        return m_props.keys();
    }

    virtual QVariantMap getProperties()
    {
        QMutexLocker locker(&m_mutex);
        return m_props;
    }

    virtual void clear()
    {
        QMutexLocker locker(&m_mutex);
        m_props.clear();
    }

private:
    QMutex m_mutex;
    QMap<QString, QVariant> m_props;
};

#endif // QPROPERTYSET_H
