#include "qtcom.h"
#include"qunknwn.h"
#include"qclassfactoryimpl.h"
#include"qrunningobjecttable.h"
#include"qclasscontainer.h"
#include"qtcomapplication.h"

QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_RunningObjectTable, QRunningObjectTableImpl)
    QTCOM_CLIDMAPENTRY(CLSID_ClassContainer, QClassContainer)
    QTCOM_CLIDMAPENTRY(CLSID_QtApplaction, QtComApplication)
QTCOM_END_CLIDMAP

QTCOM_EXPORT Qtcom::Qtcom()
{

}
