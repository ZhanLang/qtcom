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

    QHRESULT init_class( QIUnknown* pRot, QIUnknown* punkOuter)
    {
        return QS_OK;
    }
    ~QUnKnown()
    {

    }
};

QT_DEFINE_GUID(CLSID_TestClass, 0xe9678781, 0xa3cb, 0x46fb, 0x91, 0x21, 0x3e, 0xd2, 0x2c, 0x24, 0xcf, 0xad);

QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_TestClass, QUnKnown)
    QTCOM_CLIDMAPENTRY(CLSID_RunningObjectTable, QRunningObjectTableImpl)
QTCOM_END_CLIDMAP

int main(int argc, char *argv[])
{
{

    QComPtr<QIUnknown> p = new QUnKnown();
}
    QUuid u1 = CLSID_TestClass;
    qDebug() << u1.toString();

    QUuid u =  qt_uuidof(QIUnknown);
    qDebug() << u.toString();
    QApplication a(argc, argv);

    QComPtr<QIRunningObjectTable> pRot;
    QClassObjectsContainer container;
    container.Register(CLSID_RunningObjectTable, "D:\\code\\qtcom\\bin\\qwlcom1.dll");
    container.CreateInstance(CLSID_RunningObjectTable, QINull, QINull, qt_uuidof(QIRunningObjectTable),(void**)&pRot.m_p );


   
    return a.exec();
}
