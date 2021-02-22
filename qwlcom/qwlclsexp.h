#ifndef __CLS_FACTORY_H__
#define __CLS_FACTORY_H__

#include <vector>
#include "qwlinterface.h"
#include "qwlcomstd.h"

using namespace qtcom ;


class classdef
{
public:
    classdef(REFGUID _clsid, HRESULT (*pfunc)(const IID&, void**), const char* progid)
    {
        clsid = _clsid;
        pfnGetClassObject = pfunc;
        strcpy_s( ProgID,MAX_PROGIDLEN, progid);
    }

    GUID clsid;
    HRESULT (*pfnGetClassObject)(const IID&, void**);
    char ProgID[MAX_PROGIDLEN];
};

extern std::vector<classdef> g_mapClassObject;

#define BEGIN_CLIDMAP \
    std::vector<classdef> g_mapClassObject;\

#define CLIDMAPENTRY_BEGIN \
    class CLSIDExport{\
        public:\
        CLSIDExport()\
        {\
            g_mapClassObject.push_back(classdef(CLSID_WLClassFactory,&TStdClsFactory<QNullObjcetUnkown>::GetClassObject, ""));

#define CLIDMAPENTRY(CID,CLASS) \
     g_mapClassObject.push_back(classdef(CID, &TStdClsFactory<CLASS >::GetClassObject, ""));

#define CLIDMAPENTRY_NOROT(CID,CLASS) \
    g_mapClassObject.push_back(classdef(CID, &TClsFactory<CLASS >::GetClassObject, ""));

#define CLIDMAPENTRY_NOROT_PROGID(CID,CLASS,PROGID) \
    g_mapClassObject.push_back(classdef(CID, &TClsFactory<CLASS >::GetClassObject, PROGID));

#define CLIDMAPENTRY_PROGID(CID,CLASS,PROGID) \
    g_mapClassObject.push_back(classdef(CID, &TStdClsFactory<CLASS >::GetClassObject, PROGID));


#define CLIDMAPENTRY_END }}; CLSIDExport globalExport;

#define END_CLIDMAP ;

template<class CLS, class IFactory = IClassFactory>
class TClsFactory : public IFactory, public CUnknownImp
{
public: // IWLComBase:
    UNKNOWN_IMP1(IClassFactory)
public:

    // IClassFactory
    STDMETHOD (CreateInstance)(
        /* [unique][in] */ IWLComBase *punkOuter,
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
    static HRESULT create_instance(IWLComBase *punkOuter, const IID& riid, void **ppv)
    {
        RASSERT(ppv, E_INVALIDARG);


        *ppv = 0;
        RASSERT(!punkOuter || re_uuidof(IWLComBase) == riid, E_INVALIDARG);
        sentry<CLS*> p(new CLS);
        RASSERT(p, E_UNEXPECTED);
        RFAILED(p->init_class(INULL, punkOuter));
        RFAILED(p->QueryInterface(riid, ppv));
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
class TStdClsFactory : public TClsFactory<CLS, IWLClassFactory>
{
public: // IWLComBase:
    UNKNOWN_IMP2(IWLClassFactory, IClassFactory)

public: // IMSClassFactory:
    STDMETHOD(CreateInstance)(IWLComBase *prot, IWLComBase *punkOuter, const IID& riid, void **ppv)
    {
        return create_instance(prot, punkOuter, riid, ppv);
    }

    // std factory invoke:
    STDMETHOD(init_class)(IWLComBase* prot, IWLComBase* punkOuter)
    {
        return !punkOuter ? S_OK : E_INVALIDARG;
    }

    STDMETHOD(init_class_inner)(IWLComBase* punkOuter)
    {
        return !punkOuter ? S_OK : E_INVALIDARG;
    }

    // IMSClassFactory:
    STDMETHOD_(CLSID, GetAt)(LONG nIndex)
    {
        if( g_mapClassObject.size() < nIndex )
            return GUID_NULL;

        return g_mapClassObject[nIndex+1].clsid;

    }

    STDMETHOD_(LONG, GetCount)()
    {
        return g_mapClassObject.size();
        return 0;
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
    static HRESULT create_instance(IWLComBase *prot, IWLComBase *punkOuter, const IID& riid, void **ppv)
    {
        RASSERT(ppv, E_INVALIDARG);
        *ppv = 0;
        RASSERT(!punkOuter || re_uuidof(IWLComBase) == riid, E_INVALIDARG);
        sentry<CLS*> p(new CLS);
        RASSERT(p, E_UNEXPECTED);
        RFAILED(p->init_class(prot, punkOuter));
        RFAILED(p->QueryInterface(riid, ppv));
        p.detach();
        return S_OK;
    }

    static HRESULT GetClassObject(const IID& riid, void **ppv)
    {
        return TStdClsFactory<TStdClsFactory<CLS> >::create_instance(0, 0, riid, ppv);
    }
};


#endif
