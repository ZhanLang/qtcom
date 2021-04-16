#ifndef QCOMLIBRARY_H
#define QCOMLIBRARY_H
#include<QLibrary>
#include"qunknwnimpl.h"

class QComLibrary : public QLibrary
{
    typedef int (*DllGetClassObjectFunc)(const QCLSID& , const QIID& , void** );
public:
    QComLibrary():m_func(nullptr)
    {

    }

    ~QComLibrary()
    {
        QLibrary::unload();
    }

    bool open()
    {

        if( !QLibrary::load() )
            return false;

        m_func = (DllGetClassObjectFunc) QLibrary::resolve("DllGetClassObject");
        if( !m_func )
            return false;

        return true;
    }

    void close()
    {
        QLibrary::unload();
    }

    QHRESULT DllGetClassObject(const QCLSID& clsid, const QIID& iid, void** pCls)
    {
        if( m_func )
            return m_func( clsid, iid, pCls);

        return QE_NOTFIND;
    }


private:
    DllGetClassObjectFunc m_func;

};

#endif // QCOMLIBRARY_H
