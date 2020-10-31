#ifndef __COMIFACE_H__
#define __COMIFACE_H__

#include "comdef.h"

#ifndef WIN32
struct IUnknown
{
    virtual ULONG   STDMETHODCALLTYPE AddRef( void) = 0;
    virtual ULONG   STDMETHODCALLTYPE Release( void) = 0;
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void  **ppvObject) = 0;
};
MS_DEFINE_IID(IUnknown,"{00000000-0000-0000-C000-000000000046}");

struct IClassFactory : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE CreateInstance( IUnknown *pUnkOuter,REFIID riid, void **ppvObject) = 0;
    virtual HRESULT STDMETHODCALLTYPE LockServer( BOOL fLock) = 0;
};
MS_DEFINE_IID(IClassFactory,"{00000001-0000-0000-C000-000000000046}");
#endif

#define IMSBase IUnknown
#define INULL	((IUnknown*)0)

struct IMSClassFactory: public IClassFactory
{
public:
    STDMETHOD(CreateInstance)(IMSBase *prot, IMSBase *punkOuter, const IID& riid, void **ppvObject) = 0;
    STDMETHOD_(CLSID, GetAt)(LONG nIndex) = 0;
    STDMETHOD_(LONG, GetCount)() = 0;
    STDMETHOD_(const char*, ProgIDFromCLSID)(REFCLSID clsid) = 0;

};
MS_DEFINE_IID(IMSClassFactory,"{6966E385-DBFA-4131-A29E-D0E9464F3F53}");

// {E9678781-A3CB-46fb-9121-3ED22C24CFAD}
MS_DEFINE_GUID(CLSID_MSClassFactory,
               0xe9678781, 0xa3cb, 0x46fb, 0x91, 0x21, 0x3e, 0xd2, 0x2c, 0x24, 0xcf, 0xad);
#endif
