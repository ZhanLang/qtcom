#include "qtcom.h"
#include"qunknwn.h"
#include"qclassfactoryimpl.h"
#include"qrunningobjecttable.h"
#include"qclassobjectcontainer.h"

QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_RunningObjectTable, QRunningObjectTableImpl)
    QTCOM_CLIDMAPENTRY(CLSID_ClassObjects, QClassObjectsContainer)
QTCOM_END_CLIDMAP

QTCOM_EXPORT Qtcom::Qtcom()
{

}
