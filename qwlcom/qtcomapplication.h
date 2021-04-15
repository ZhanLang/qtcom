#ifndef QTCOMAPPLICATION_H
#define QTCOMAPPLICATION_H
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
    QComPtr<QIRunningObjectTable> m_rot;
    QComPtr<QIPropertySet> m_prop;
};

#endif // QTCOMAPPLICATION_H
