#ifndef __CLS_FACTORY_H__
#define __CLS_FACTORY_H__

#include "comiface.h"
#include "comstd.h"

template<class CLS, class IFactory = IClassFactory>
class TClsFactory : public IFactory, public CUnknownImp
{
public: // IMSBase:
    UNKNOWN_IMP1(IClassFactory);

public:
    // IClassFactory
    STDMETHOD (CreateInstance)(
        /* [unique][in] */ IMSBase *punkOuter,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppv)
    {
        return create_instance(punkOuter, riid, ppv);
    }

    STDMETHOD(LockServer)(BOOL fLock)
    {
        return lock_server(fLock);
    }

public:
    static HRESULT create_instance(IMSBase *punkOuter, const IID& riid, void **ppv)
    {
        *ppv = 0;
        // aggregation validate:
        RASSERT(!punkOuter || re_uuidof(IMSBase) == riid, E_INVALIDARG);
        // create new object/aggregation:
        sentry<CLS*> p(new CLS);
        RASSERT(p, E_UNEXPECTED);
        ((IUnknown_Nondelegate*)(CLS*)p)->AddRef_Nondelegate(); // nondelegation, protect reference count
        if(punkOuter)
        {
            CUnknownImp_Inner* pInner = (CUnknownImp_Inner*)(p.m_p);
            RFAILED(pInner->init_class_inner(punkOuter));
        }
        HRESULT hr = ((IUnknown_Nondelegate*)(CLS*)p)->QueryInterface_Nondelegate(riid, ppv);
        ((IUnknown_Nondelegate*)p.detach())->Release_Nondelegate(); // nondelegation, balance reference count or destroy.
        return hr;
    }

    static HRESULT lock_server(BOOL /*fLock*/)
    {
        return S_OK;
    }

    static HRESULT GetClassObject(const IID& riid, void **ppv)
    {
        return TClsFactory<TClsFactory<CLS> >::create_instance(0, riid, ppv);
    }
};


template<class CLS>
class TStdClsFactory : public TClsFactory<CLS, IMSClassFactory>
{
public: // IMSBase:
    UNKNOWN_IMP2(IMSClassFactory, IClassFactory);

public: // IMSClassFactory:
    STDMETHOD(CreateInstance)(IMSBase *prot, IMSBase *punkOuter, const IID& riid, void **ppv)
    {
        return create_instance(prot, punkOuter, riid, ppv);
    }

    // std factory invoke:
    STDMETHOD(init_class)(IMSBase* prot, IMSBase* punkOuter)
    {
        return !punkOuter ? S_OK : E_INVALIDARG;
    }

    STDMETHOD(init_class_inner)(IMSBase* punkOuter)
    {
        return !punkOuter ? S_OK : E_INVALIDARG;
    }

    // IMSClassFactory:
    STDMETHOD_(CLSID, GetAt)(LONG nIndex)
    {
        return g_mapClassObject[nIndex+1].clsid;
    }

    STDMETHOD_(LONG, GetCount)()
    {
        LONG lCount = sizeof(g_mapClassObject)/sizeof(g_mapClassObject[0]);

        return (lCount > 0)?lCount-1:0;
    }


    STDMETHOD_(LPCTSTR, ProgIDFromCLSID)(REFCLSID clsid)
    {
        for(size_t i = 1; i < sizeof(g_mapClassObject)/sizeof(g_mapClassObject[0]); ++i)
        {
            if(clsid == g_mapClassObject[i].clsid)
            {
                return g_mapClassObject[i].ProgID;
            }
        }
        return _T("");
    }

public:
    static HRESULT create_instance(IMSBase *prot, IMSBase *punkOuter, const IID& riid, void **ppv)
    {
        *ppv = 0;
        // aggregation validate:
        RASSERT(!punkOuter || re_uuidof(IMSBase) == riid, E_INVALIDARG);
        // create new object/aggregation:
        sentry<CLS*> p(new CLS);
        RASSERT(p, E_UNEXPECTED);
        ((IUnknown_Nondelegate*)(CLS*)p)->AddRef_Nondelegate(); // nondelegation, protect reference count
        RFAILED(p->init_class(prot, punkOuter));
        HRESULT hr = ((IUnknown_Nondelegate*)(CLS*)p)->QueryInterface_Nondelegate(riid, ppv);
        ((IUnknown_Nondelegate*)p.detach())->Release_Nondelegate(); // nondelegation, balance reference count or destroy.
        return hr;
    }

    static HRESULT GetClassObject(const IID& riid, void **ppv)
    {
        return TStdClsFactory<TStdClsFactory<CLS> >::create_instance(0, 0, riid, ppv);
    }
};


#define BEGIN_CLIDMAP \
    static const struct{ CLSID clsid; HRESULT (*pfnGetClassObject)(const IID&, void**);TCHAR ProgID[MAX_PROGIDLEN];} g_mapClassObject[] = {

#define CLIDMAPENTRY_BEGIN \
    {CLSID_MSClassFactory,&TStdClsFactory<CNullObjcetUnkown>::GetClassObject, _T("")},

#define CLIDMAPENTRY(CID,CLASS) \
    {CID, &TStdClsFactory<CLASS >::GetClassObject, _T("")},

#define CLIDMAPENTRY_NOROT(CID,CLASS) \
    {CID, &TClsFactory<CLASS >::GetClassObject, _T("")},

#define CLIDMAPENTRY_NOROT_PROGID(CID,CLASS,PROGID) \
{CID, &TClsFactory< CLASS >::GetClassObject, PROGID},

#define CLIDMAPENTRY_PROGID(CID,CLASS,PROGID) \
    {CID, &TStdClsFactory<CLASS >::GetClassObject, PROGID},


#define CLIDMAPENTRY_END

#define END_CLIDMAP };

#endif
