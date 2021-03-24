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

    QHRESULT open( const QString& fileName)
    {
        if( !QLibrary::isLibrary(fileName ))
            return QE_UNEXPECTED;

        QLibrary::setFileName(fileName);
        if( !QLibrary::load() )
            return QE_FAIL;

        m_func = (DllGetClassObjectFunc) QLibrary::resolve("DllGetClassObject");
        if( m_func )
            return QS_OK;

        return QE_NOINTERFACE;
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
