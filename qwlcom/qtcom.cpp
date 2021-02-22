#include "qtcom.h"

#include"combase.h"
#include"qwlinterface.h"
#include "qwlcomptr.h"
#include"qwlcomstd.h"
#include"qtcomexp.h"
#include"qwlclsexp.h"
#include"guidfun.h"
#include "tproperty.h"
#include"tproperty.impl.h"

using namespace qtcom;
BEGIN_CLIDMAP
CLIDMAPENTRY_BEGIN
    CLIDMAPENTRY_PROGID(CLSID_QProperty,            QProperty,          "Property.1")
    CLIDMAPENTRY_PROGID(CLSID_QProperty_thread,     QPropertyThread,    "PropertyThread.1")
    CLIDMAPENTRY_PROGID(CLSID_QProperty2,           QProperty2,         "Property2.1")
    CLIDMAPENTRY_PROGID(CLSID_QProperty2_thread,    QProperty2Thread,   "Property2Thread.1")
    CLIDMAPENTRY_PROGID(CLSID_QPropertyStr,         QPropertyStr,       "PropertyStr.1")
    CLIDMAPENTRY_PROGID(CLSID_QPropertyStr_thread,  QPropertyStrThread, "PropertyStrThread.1")
CLIDMAPENTRY_END
DEFINE_ALL_EXPORTFUN
