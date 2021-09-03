#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "testmodule_global.h"

#include <QIUnknown.h>
#include <QIUnknownImpl.h>
#include "../QIPrintHelloWord.h"
class  Testmodule : public QIPrintHelloWord, public QIUnknownImpl
{
public:
    Testmodule();
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIPrintHelloWord)
    QTCOM_QUERYINTERFACE_END;

    virtual QHRESULT Print();
};

#endif // TESTMODULE_H
