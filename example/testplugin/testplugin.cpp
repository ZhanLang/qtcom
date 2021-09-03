#include "testplugin.h"

#include<QClassFactoryImpl.h>
#include <QDebug>
#include<QIApplication.h>
QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_TestPlugin, Testplugin)
QTCOM_END_CLIDMAP

#include"../QIPrintHelloWord.h"
Testplugin::Testplugin()
{
}

QHRESULT Testplugin::Init(QIPropertySet * prop)
{
    qDebug() << "Testplugin Init";

    //获取应用程序的初始配置
    QtComPtr<QIApplication> app;
    m_rot->GetObject(CLSID_QtApplaction, qt_uuidof(QIApplication), (void**)&app.m_p);
    qDebug() << app->getPropertySet()->getProperties();


    //获取当前插件的初始化设置
    qDebug() << prop->getProperties();
    QtComPtr<QIPrintHelloWord> print;
    QHRESULT hr = m_rot->CreateInstance(CLSID_PrintHelloWord, qt_uuidof(QIPrintHelloWord), (void**)&print.m_p);
    if( QFAILED(hr) )
        return hr;

    print->Print();
    return QS_OK;
}

QHRESULT Testplugin::UnInit(int returnCode)
{
    qDebug() << "Testplugin UnInit";
    return QS_OK;
}

QHRESULT Testplugin::Start()
{
    qDebug() << "Testplugin Start";
    return QS_OK;
}

QHRESULT Testplugin::Stop()
{
    qDebug() << "Testplugin Stop";
    return QS_OK;
}


QHRESULT Testplugin::init_class(QIUnknown *rot, QIUnknown *outer)
{
    if( !rot )
        return QE_INVALIDARG;

    m_rot = rot;
    return QS_OK;
}
