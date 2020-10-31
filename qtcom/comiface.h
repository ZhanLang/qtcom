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
MS_DEFINE_IID(IUnknown,"{00000001-0000-0000-C000-000000000046}");
#endif

#define IMSBase IUnknown
#define INULL	((IUnknown*)0)

interface IMSClassFactory: public IClassFactory
{
public:
    STDMETHOD(CreateInstance)(IMSBase *prot, IMSBase *punkOuter, const IID& riid, void **ppvObject) = 0;
    STDMETHOD_(CLSID, GetAt)(LONG nIndex) = 0;
    STDMETHOD_(LONG, GetCount)() = 0;
    STDMETHOD_(LPCTSTR, ProgIDFromCLSID)(REFCLSID clsid) = 0;

};
MS_DEFINE_IID(IMSClassFactory,"{6966E385-DBFA-4131-A29E-D0E9464F3F53}");

#endif
