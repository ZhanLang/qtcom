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



int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    QtComPtr<QIRunningObjectTable> pRot;

    QComLibrary* lib = new QComLibrary();
    lib->setFileName("D:\\code\\qtcom\\bin\\qwlcom.dll");

    lib->open();

    QtComPtr<QIClassFactory> pCls;
    lib->DllGetClassObject(CLSID_QtApplaction, qt_uuidof(QIClassFactory), (void**)&pCls.m_p);
    if( pCls )
    {
        QtComPtr<QIApplication> pApp;
        pCls->CreateInstance(qt_uuidof(QIApplication), (void**)&pApp.m_p);

        pApp->setConfigureFile(".\\qtcom.example.cfg.json");
    }

    

   
    return a.exec();
}
