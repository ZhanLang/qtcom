#ifndef _Q_IUNKNOWN_IMPL_H_
#define _Q_IUNKNOWN_IMPL_H_

#include"QIUnknown.h"
#include<QAtomicInteger>

class QIUnknownImpl
{
public:
    QIUnknownImpl(): m_RefCount(0) {}
    virtual ~QIUnknownImpl(){}

    virtual QHRESULT init_class( QIUnknown* rot, QIUnknown* outer)
    {
        Q_UNUSED( rot )
        Q_UNUSED( outer )

        return QS_OK;
    }

protected:
    QAtomicInteger<ulong> m_RefCount;
};


#endif
