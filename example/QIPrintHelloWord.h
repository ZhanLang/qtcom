
#include <QIUnknown.h>
struct QIPrintHelloWord : public QIUnknown
{
    virtual QHRESULT Print() = 0;
};

QT_DEFINE_IID(QIPrintHelloWord, "qtcom.i.printhelloword");

QT_DEFINE_CLSID(CLSID_PrintHelloWord,"qtcom.class.printhelloword");
