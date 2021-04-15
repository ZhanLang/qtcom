#include "qtcombase.h"

#include<QClassFactoryImpl>
#include<QPropertySet>
#include"qbufferimpl.h"

QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_QPropertySet, QPropertySet)
    QTCOM_CLIDMAPENTRY(CLSID_Buffer , QBufferImp<QNullCriticalSection>)
    QTCOM_CLIDMAPENTRY(CLSID_SafeBuffer ,QBufferImp<QAutoCriticalSection>)
QTCOM_END_CLIDMAP
