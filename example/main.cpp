#include "example.h"
#include <QApplication>
#include<QIUnknown>
#include<QIUnknownImpl>
#include<QComPtr>
#include<QDebug>
#include<QClassFactoryImpl>
#include"../qwlcom/qrunningobjecttable.h"
#include"../qwlcom/qclassobjectcontainer.h"

class QUnKnown : public QIUnknown, public QUnknownImp
{
public:
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIUnknown)
        QTCOM_QUERYINTERFACE_ENTRY(QIUnknown)
    QTCOM_QUERYINTERFACE_END

    QUnKnown( void* parent)
    {

    }
    QHRESULT init_class( QIUnknown* pRot, QIUnknown* punkOuter)
    {
        return QS_OK;
    }
    ~QUnKnown()
    {

    }
};

QT_DEFINE_CLSID(CLSID_TestClass, "CCC");

QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_RunningObjectTable, QRunningObjectTableImpl)
QTCOM_END_CLIDMAP

int main(int argc, char *argv[])
{
{

   // QComPtr<QIUnknown> p = new QUnKnown(nullptr);
}
    QIID u1 = CLSID_TestClass;


    QIID u =  qt_uuidof(QIUnknown);

    QApplication a(argc, argv);

    QComPtr<QIRunningObjectTable> pRot;
    QClassContainer container(nullptr);
    container.Register(CLSID_RunningObjectTable, "D:\\code\\qtcom\\bin\\qwlcom1.dll");
   // container.CreateInstance(CLSID_RunningObjectTable, QINull, QINull, qt_uuidof(QIRunningObjectTable),(void**)&pRot.m_p );

    {
        QComPtr<QIClassFactory> pCls;
        DllGetClassObject(CLSID_TestClass, qt_uuidof(QIClassFactory), (void**)&pCls.m_p);

        QComPtr<QIUnknown> pUnknown;
        pCls->CreateInstance(qt_uuidof(QIUnknown), (void**)pCls.m_p);
    }

   
    return a.exec();
}
