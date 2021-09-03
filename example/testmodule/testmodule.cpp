#include "testmodule.h"

#include <QDebug>
Testmodule::Testmodule()
{
}

QHRESULT Testmodule::Print()
{
    qDebug() << "hello word.";
    return QS_OK;
}

#include <QClassFactoryImpl.h>

QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_PrintHelloWord, Testmodule)
QTCOM_END_CLIDMAP
