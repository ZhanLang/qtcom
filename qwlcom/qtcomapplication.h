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

    QHRESULT init_class( QIUnknown* , QIUnknown* );

    QtComApplication( void * );
    ~QtComApplication();

protected:
    QSTDMETHOD(setConfigureFile)(const QString& cfgfile);
    QSTDMETHOD(Exec)(int argc, char *argv[]);
    QSTDMETHOD(Quit)(int returnCode);

    QSTDMETHOD_(QIPropertySet*,getPropertySet)();
    QSTDMETHOD_(QIRunningObjectTable*,getRunningObjectTable)();

    QSTDMETHOD(setConfigure)(const QByteArray& cfgfile);
private:
    QHRESULT load(const QString& cfgFile);
    void initPropterty( QJsonDocument& doc);
    QHRESULT initModules(QJsonDocument& doc);
    QHRESULT initPlugins(QJsonDocument& doc);

    QStringList enumerate_jsonfile(const QString& path);
private:
    QtComPtr<QIRunningObjectTable> m_rot;
    QtComPtr<QIPropertySet> m_prop;
    QtComPtr<QIClassContainer> m_cclass;
    QtComPtr<QIPluginContainer> m_plugins;
};

#endif // QTCOMAPPLICATION_H
