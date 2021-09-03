#ifndef _Q_I_CLASS_FACTORY_H_
#define _Q_I_CLASS_FACTORY_H_

#include"QIUnknown.h"

struct QIClassFactory : public QIUnknown
{
public:
   virtual QHRESULT CreateInstance(const QIID& iid, void **ppv, QIUnknown *prot = QINull, QIUnknown *pUnkOuter = QINull) = 0;
};
QT_DEFINE_IID(QIClassFactory,"qtcom.i.class_factory");


#endif
