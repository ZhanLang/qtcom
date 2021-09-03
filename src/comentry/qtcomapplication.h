#ifndef QTCOMAPPLICATION_H
#define QTCOMAPPLICATION_H

#include<QIUnknownImpl.h>
#include<QIApplication.h>
#include<QtComPtr.h>
#include<QIClassContainer.h>
#include"qplugincontainer.h"

class QtComApplication : public QIApplication, public QIUnknownImpl
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIApplication)
    QTCOM_QUERYINTERFACE_END

    QHRESULT init_class( QIUnknown* , QIUnknown* );

    QtComApplication( );
    virtual ~QtComApplication();

protected:
    virtual QHRESULT setConfigureFile(const QString& cfgfile);
    virtual QHRESULT Exec(int argc, char *argv[]);
    virtual QHRESULT Quit(int returnCode);

    virtual QIPropertySet* getPropertySet();
    virtual QIRunningObjectTable* getRunningObjectTable();

    QSTDMETHOD(setConfigure)(const QByteArray& cfgfile);

    virtual QString getCurrentPath();
    virtual QString getCurrentDir();

private:
    QHRESULT load(const QString& cfgFile);

    QHRESULT addMoudle(const QString& name, const QString& path);
    QHRESULT addPlugin(const QString& name, const QString& path);

    QStringList enumerate_xmlfile(const QString& path);
private:
    QtComPtr<QIRunningObjectTable> m_rot;
    QtComPtr<QIPropertySet> m_prop;
    QtComPtr<QIClassContainer> m_cclass;
    QPluginContainer* m_plugins;
    QString m_currentPath;
    QString m_currentDir;
};

#endif // QTCOMAPPLICATION_H
