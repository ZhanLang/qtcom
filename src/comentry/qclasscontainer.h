#ifndef QCLASSOBJECTS_H
#define QCLASSOBJECTS_H
#include <QHash>
#include<QMutex>
#include<QFile>
#include<QLibrary>
#include<QIClassContainer.h>
#include<QtComPtr.h>
#include<QIUnknownImpl.h>

typedef int (*QtDllGetClassObjectFunc)(const QCLSID&, const QIID&, void**);
class QClassContainer : public QIClassContainer , private QIUnknownImpl
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIClassContainer)
    QTCOM_QUERYINTERFACE_END
    QHRESULT init_class( QIUnknown*, QIUnknown*);

    QClassContainer();
    virtual ~QClassContainer();


    QSTDMETHOD(CreateInstance)(const QCLSID& clsid, const QIID& iid, void **ppv, QIUnknown *prot = QINull, QIUnknown *pUnkOuter = QINull);

    QSTDMETHOD(GetClassObject)(const QCLSID& clsid, const QIID& iid,void **ppv);

    QSTDMETHOD(Register)(const QCLSID& clsid, const QString& path, const QString& libName);

    QSTDMETHOD(registerModules)(const QByteArray& cfg);

    QSTDMETHOD(registerModulesFile)(const QString& file);
    QSTDMETHOD(registerModulesFiles)(const QStringList& files);

    QSTDMETHOD_(bool,isRegistered)(const QCLSID& clsid);

    QSTDMETHOD(Revoke)(const QCLSID& clsid);

    QSTDMETHOD_(void,RevokeAll)();

private:
    QMutex m_mutex;
    QHash<QCLSID, QString> m_clsobjs;
};

#endif // QCLASSOBJECTS_H
