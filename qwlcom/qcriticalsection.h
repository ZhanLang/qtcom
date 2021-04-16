

#ifndef _CRITICALSECTION_INCLUDE_H_
#define _CRITICALSECTION_INCLUDE_H_

#ifdef _WIN32
#pragma once
#endif

#include<QMutex>


class QNullCriticalSection
{
public:
    void Lock() {}
    void Unlock() {}
};

class QAutoCriticalSection
{
public:
    void Lock() {m_sec.lock();}
    void Unlock() {m_sec.unlock();}

private:
    QMutex m_sec;
};




template <class T>
class QStackLockWrapper
{
public:
    QStackLockWrapper(T& _t) : t(_t)
    {
        t.Lock();
    }
    ~QStackLockWrapper()
    {
        t.Unlock();
    }
    T &t;
};


typedef QStackLockWrapper<QAutoCriticalSection> QStackAutoCSLock;


//help macro:
#define	DECLARE_AUTOLOCK_CS(name)				private : CAutoCriticalSection	m_cs ## name
#define AUTOLOCK_CS(name)						CStackAutoCSLock lock(m_cs ## name)
#define AUTOLOCK_CS_LOCK(name)					m_cs ## name.Lock()		
#define AUTOLOCK_CS_UNLOCK(name)				m_cs ## name.Unlock()

#endif // _CRITICALSECTION_INCLUDE_H_
