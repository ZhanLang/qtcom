#include "qtcom.h"
#include"qunknwn.h"
#include"qclassfactoryimpl.h"
#include"qrunningobjecttable.h"
#include"qclasscontainer.h"
#include"qtcomapplication.h"
#include"qplugincontainer.h"
#include"qpropertyset.h"

QTCOM_BEGIN_CLIDMAP
    QTCOM_CLIDMAPENTRY(CLSID_QRunningObjectTable, QRunningObjectTableImpl)
    QTCOM_CLIDMAPENTRY(CLSID_QClassContainer, QClassContainer)
    QTCOM_CLIDMAPENTRY(CLSID_QPropertySet, QPropertySet)
    QTCOM_CLIDMAPENTRY(CLSID_QtApplaction, QtComApplication)
    QTCOM_CLIDMAPENTRY(CLSID_QPluginContainer, QPluginContainer)
QTCOM_END_CLIDMAP
