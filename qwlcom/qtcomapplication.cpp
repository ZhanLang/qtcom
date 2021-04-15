#include "qtcomapplication.h"

#include"qrunningobjecttable.h"
#include"qpropertyset.h"
QtComApplication::QtComApplication(void * )
{
    m_rot = new QRunningObjectTableImpl(nullptr);
    m_prop = new QPropertySet(nullptr);
}

QtComApplication::~QtComApplication()
{
    m_rot = QINull;
    m_prop = QINull;
}

QSTDMETHOD_IMPL QtComApplication::Exec(const QString& cfgFile)
{
    return QS_OK;
}

QSTDMETHOD_IMPL QtComApplication::Quit(int returnCode)
{
    return QS_OK;
}

QSTDMETHOD_IMPL_ (QIPropertySet*) QtComApplication::getPropertySet()
{
    return m_prop;
}

QSTDMETHOD_IMPL_(QIRunningObjectTable*) QtComApplication::getRunningObjectTable()
{
    return m_rot.m_p;
}

