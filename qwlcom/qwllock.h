#ifndef __QWLLOCK_H_
#define __QWLLOCK_H_
#include <QMutex>
#include "qwltype.h"

namespace qtcom {
class QNullCriticalSection
{
public:
    void Lock(DWORD) {}
    void Unlock() {}
    QNullCriticalSection() {}
    ~QNullCriticalSection() {}
};

class QAutoCriticalSection
{
public:
    void Lock(DWORD dwTimeout = -1) {m_mutex.lock();}
    void Unlock() {m_mutex.unlock();}
    QAutoCriticalSection() {}
    ~QAutoCriticalSection() {}

    QMutex m_mutex;
};

template <class T,DWORD Timeout=INFINITE>
class QStackLockWrapper
{
    public:
        QStackLockWrapper(T& _t) : t(_t)
        {
            t.Lock(Timeout);
        }
        ~QStackLockWrapper()
        {
            t.Unlock();
        }
        T &t;
};

//help macro:
#define	DECLARE_AUTOLOCK_CS(name)				private : QAutoCriticalSection	m_cs ## name
#define AUTOLOCK_CS(name)						QStackAutoCSLock lock(m_cs ## name)
#define AUTOLOCK_CS_LOCK(name)					m_cs ## name.Lock()
#define AUTOLOCK_CS_UNLOCK(name)				m_cs ## name.Unlock()
};
#endif
