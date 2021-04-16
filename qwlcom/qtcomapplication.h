#ifndef QTCOMAPPLICATION_H
#define QTCOMAPPLICATION_H

#include<QJsonDocument>
#include<QJsonObject>
#include"qunknwnimpl.h"
#include"qcomptr.h"
class QtComApplication : public QIApplication, public QUnknownImp
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIApplication)
        QTCOM_QUERYINTERFACE_ENTRY(QIApplication)
    QTCOM_QUERYINTERFACE_END

    QHRESULT init_class( QIUnknown* , QIUnknown* )
    {
        return QS_OK;
    }

    QtComApplication( void * );
    ~QtComApplication();

protected:
    QSTDMETHOD(Exec)(const QString& cfgFile);
    QSTDMETHOD(Quit)(int returnCode);

    QSTDMETHOD_(QIPropertySet*,getPropertySet)();
    QSTDMETHOD_(QIRunningObjectTable*,getRunningObjectTable)();

private:
    QHRESULT load(const QString& cfgFile);
    void initPropterty( QJsonDocument& doc);
    QHRESULT loadModule(const QString& file);
    QHRESULT loadClass(const QString& cfgPath, QJsonObject& doc);
private:
    QComPtr<QIRunningObjectTable> m_rot;
    QComPtr<QIPropertySet> m_prop;
    QComPtr<QIClassContainer> m_clsContainer;
};

#endif // QTCOMAPPLICATION_H
