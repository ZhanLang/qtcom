#ifndef __CLS_FACTORY_H__
#define __CLS_FACTORY_H__

#include <vector>
#include "comiface.h"
#include "comstd.h"

struct ClsdDefine
{
    ClsdDefine(REFGUID _clsid, HRESULT (*pfunc)(const IID&, void**), const char* progid)
    {
        clsid = _clsid;
        pfnGetClassObject = pfunc;
        strcpy( ProgID, progid);
    }

    GUID clsid;
    HRESULT (*pfnGetClassObject)(const IID&, void**);
    char ProgID[MAX_PROGIDLEN];
};

extern std::vector<ClsdDefine> g_mapClassObject;

#define BEGIN_CLIDMAP \
     std::vector<ClsdDefine> g_mapClassObject;

#define CLIDMAPENTRY_BEGIN \
     class clsCNullObjcetUnkown{public:clsCNullObjcetUnkown(){ g_mapClassObject.push_back(ClsdDefine{CLSID_MSClassFactory,&TStdClsFactory<CNullObjcetUnkown>::GetClassObject, ""});}} impCNullObjcetUnkown;

#define CLIDMAPENTRY(CID,CLASS) \
    class cls##CLASS{public:cls##CLASS(){ g_mapClassObject.push_back(ClsdDefine(CID, &TStdClsFactory<CLASS >::GetClassObject, ""));}} imp##CLASS;

#define CLIDMAPENTRY_NOROT(CID,CLASS) \
 class cls##CLASS{public:cls##CLASS(){ g_mapClassObject.push_back(ClsdDefine(CID, &TClsFactory<CLASS >::GetClassObject, ""));}} imp##CLASS;

#define CLIDMAPENTRY_NOROT_PROGID(CID,CLASS,PROGID) \
class cls##CLASS{public:cls##CLASS(){ g_mapClassObject.push_back(ClsdDefine(CID, &TClsFactory<CLASS >::GetClassObject, PROGID));}} imp##CLASS;

#define CLIDMAPENTRY_PROGID(CID,CLASS,PROGID) \
    class cls##CLASS{public:cls##CLASS(){ g_mapClassObject.push_back(ClsdDefine(CID, &TStdClsFactory<CLASS >::GetClassObject, PROGID));}} imp##CLASS;


#define CLIDMAPENTRY_END

#define END_CLIDMAP ;

template<class CLS, class IFactory = IClassFactory>
class TClsFactory : public IFactory, public CUnknownImp
{
public: // IMSBase:
    UNKNOWN_IMP1(IClassFactory)

    static std::vector<ClsdDefine> g_mapClassObject;
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
        RASSERT(ppv, E_INVALIDARG);


        *ppv = 0;
        RASSERT(!punkOuter || re_uuidof(IMSBase) == riid, E_INVALIDARG)
        sentry<CLS*> p(new CLS);
        RASSERT(p, E_UNEXPECTED)
        RFAILED(p->init_class(INULL, punkOuter))
        RFAILED(p->QueryInterface(riid, ppv))
        p.detach();
        return S_OK;
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
    UNKNOWN_IMP2(IMSClassFactory, IClassFactory)

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
        return GUID_NULL;
    }

    STDMETHOD_(LONG, GetCount)()
    {
        return g_mapClassObject.size();
    }


    STDMETHOD_(const char*, ProgIDFromCLSID)(REFCLSID clsid)
    {
        for(int i = 1; g_mapClassObject.size(); ++i)
        {
            if(clsid == g_mapClassObject[i].clsid)
            {
                return g_mapClassObject[i].ProgID;
            }
        }
        return "";
    }

public:
    static HRESULT create_instance(IMSBase *prot, IMSBase *punkOuter, const IID& riid, void **ppv)
    {
        RASSERT(ppv, E_INVALIDARG);
        *ppv = 0;
        RASSERT(!punkOuter || re_uuidof(IMSBase) == riid, E_INVALIDARG)
        sentry<CLS*> p(new CLS);
        RASSERT(p, E_UNEXPECTED)
        RFAILED(p->init_class(prot, punkOuter))
        RFAILED(p->QueryInterface(riid, ppv))
        p.detach();
        return S_OK;
    }

    static HRESULT GetClassObject(const IID& riid, void **ppv)
    {
        return TStdClsFactory<TStdClsFactory<CLS> >::create_instance(0, 0, riid, ppv);
    }
};



#endif
