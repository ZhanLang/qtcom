#include "example.h"
#include <QApplication>
#include<QIUnknown>
#include<QIUnknownImpl>
#include<QComPtr>
#include<QDebug>
#include<QClassFactoryImpl>
#include"../qwlcom/qrunningobjecttable.h"
#include"../qwlcom/qclasscontainer.h"
#include"../qwlcom/qcomlibrary.h"
#include<QTaskCommandBase.h>

typedef int (*QtDllGetClassObjectFunc)(const QCLSID& , const QIID& , void** );

class QTestCommand : public QTaskCommandBase{
public:
    QErrorCode run(){
        return QErrorCode::Ok;
    }
};

class QTestCommandContext :public QITaskCommandContext,public QUnknownImp
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QITaskCommandContext)
        QTCOM_QUERYINTERFACE_ENTRY(QITaskCommandContext)
    QTCOM_QUERYINTERFACE_END

    QTestCommandContext():m_cancel(false){

    }
    virtual bool isCancel() const{
        return m_cancel;
    }

    virtual void cancel() {
        m_cancel = true;
    }

private:
    bool m_cancel;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtComPtr<QIRunningObjectTable> pRot;


     QtComPtr< QITaskCommandContext> taskcmdContext = new QTestCommandContext();
     QtComPtr< QITaskCommand> taskcmd = new QTestCommand();
    taskcmd->addContext(taskcmdContext);
    taskcmd->removeContext(taskcmdContext);

    QFunctionPointer symbol = QLibrary::resolve("./qwlcom", "QtDllGetClassObject");
    if( !symbol )
    {
        //todo:
        return QE_RUNTIME;
    }

    QtComPtr<QIClassFactory> pCls;
    QtDllGetClassObjectFunc func = (QtDllGetClassObjectFunc)symbol;
    QHRESULT hr = func(CLSID_QtApplaction, qt_uuidof(QIClassFactory), (void**)&pCls.m_p);
    if (QFAILED(hr))
        return 0;

    QtComPtr<QIApplication> pApp;
    pCls->CreateInstance(qt_uuidof(QIApplication), (void**)&pApp.m_p);
    if (!pApp)
        return 0;

    pApp->setConfigureFile("./qtcom.example.cfg.json");

    {
        QComLibrary* lib = new QComLibrary();
        lib->setFileName("./qwlcom");

        QComLibrary* lib2 = new QComLibrary();
        lib2->setFileName("./qwlcom");

        lib->open();
        lib2->open();

        delete  lib2;
        QtComPtr<QIClassFactory> pCls;
        lib->DllGetClassObject(CLSID_QtApplaction, qt_uuidof(QIClassFactory), (void**)&pCls.m_p);
        if( pCls )
        {
            QtComPtr<QIApplication> pApp;
            pCls->CreateInstance(qt_uuidof(QIApplication), (void**)&pApp.m_p);

            pApp->setConfigureFile(".\\qtcom.example.cfg.json");
        }
    }


    

   
    return a.exec();
}
