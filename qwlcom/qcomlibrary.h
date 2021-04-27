#ifndef QCOMLIBRARY_H
#define QCOMLIBRARY_H
#include<QLibrary>
#include"qunknwnimpl.h"

class QComLibrary : public QLibrary
{
    typedef int (*QtComDllGetClassObjectFunc)(const QCLSID& , const QIID& , void** );
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

        m_func = (QtComDllGetClassObjectFunc) QLibrary::resolve("QtComDllGetClassObject");
        if( !m_func )
            return false;

        return true;
    }

    void close()
    {
        QLibrary::unload();
    }

    QHRESULT DllGetClassObject(const QCLSID& clsid, const QIID& factoryiid, void** pCls)
    {
        if( m_func )
            return m_func( clsid, factoryiid, pCls);

        return QE_NOTFIND;
    }


private:
    QtComDllGetClassObjectFunc m_func;

};

#endif // QCOMLIBRARY_H
