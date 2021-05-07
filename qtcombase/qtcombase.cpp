#include "qtcombase.h"

#include<QClassFactoryImpl>
#include<QPropertySet>
#include"qbufferimpl.h"
#include"qthreadpooltaskloopimpl.h"

QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_QPropertySet, QPropertySet)
    QTCOM_CLIDMAPENTRY(CLSID_Buffer , QBufferImp<QNullCriticalSection>)
    QTCOM_CLIDMAPENTRY(CLSID_SafeBuffer ,QBufferImp<QAutoCriticalSection>)
    QTCOM_CLIDMAPENTRY(CLSID_ThreadPoolCommandLoop ,QThreadPoolTaskLoopImpl)
QTCOM_END_CLIDMAP
