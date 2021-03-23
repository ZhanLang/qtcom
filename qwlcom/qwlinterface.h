#ifndef __QWLINTERFACE_H__
#define __QWLINTERFACE_H__

#include"qwltype.h"
#include"qwluuid.h"
#ifdef WIN32
#include <Windows.h>
#else
struct IUnknown
{
    virtual ULONG   STDMETHODCALLTYPE AddRef( void) = 0;
    virtual ULONG   STDMETHODCALLTYPE Release( void) = 0;
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void  **ppvObject) = 0;
};
QWL_DEFINE_IID(IUnknown,"{00000000-0000-0000-C000-000000000046}");

struct IClassFactory : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE CreateInstance( IUnknown *pUnkOuter,REFIID riid, void **ppvObject) = 0;
    virtual HRESULT STDMETHODCALLTYPE LockServer( BOOL fLock) = 0;
};
QWL_DEFINE_IID(IClassFactory,"{00000001-0000-0000-C000-000000000046}");
#endif

#define IWLComBase  IUnknown
#define INULL       ((IUnknown*)0)

namespace qtcom {

struct IWLClassFactory: public IClassFactory
{
public:
    STDMETHOD(CreateInstance)(IWLComBase *prot, IWLComBase *punkOuter, const IID& riid, void **ppvObject) = 0;
    STDMETHOD_(CLSID, GetAt)(LONG nIndex) = 0;
    STDMETHOD_(LONG, GetCount)() = 0;
    STDMETHOD_(const char*, ProgIDFromCLSID)(REFCLSID clsid) = 0;

};
QWL_DEFINE_IID(IWLClassFactory,"{6966E385-DBFA-4131-A29E-D0E9464F3F53}");

// {E9678781-A3CB-46fb-9121-3ED22C24CFAD}
QWL_DEFINE_GUID(CLSID_WLClassFactory,
               0xe9678781, 0xa3cb, 0x46fb, 0x91, 0x21, 0x3e, 0xd2, 0x2c, 0x24, 0xcf, 0xad);


};
#endif
