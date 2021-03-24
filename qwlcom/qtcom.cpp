#include "qtcom.h"
#include"qunknwn.h"
#include"qclassfactoryimpl.h"
#include"qrunningobjecttable.h"
#include"qclassobjectcontainer.h"

QBEGIN_CLIDMAP
    QCLIDMAPENTRY(CLSID_RunningObjectTable, QRunningObjectTableImpl)
    QCLIDMAPENTRY(CLSID_ClassObjects, QClassObjectsContainer)
QEND_CLIDMAP

QTCOM_EXPORT Qtcom::Qtcom()
{

}
