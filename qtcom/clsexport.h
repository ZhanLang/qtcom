#ifndef __CLS_EXPORT_H__
#define __CLS_EXPORT_H__

#if defined(_MSC_VER) || defined(_WIN32) || defined(_WIN64)
#define TCE_API __stdcall
#define DLL_API __declspec(dllexport)
#else
#define TCE_API
#define DLL_API
#endif

#define EXPORT_API extern "C"  HRESULT TCE_API


#define DEFINE_ALL_EXPORTFUN \
        EXPORT_API DllGetClassObject(IN REFCLSID rclsid, IN REFIID riid, OUT LPVOID FAR* ppv)\
{\
        for(size_t i = 0; i < sizeof(g_mapClassObject)/sizeof(g_mapClassObject[0]); ++i)\
        {\
                if(rclsid == g_mapClassObject[i].clsid)\
                {\
                        return g_mapClassObject[i].pfnGetClassObject(riid, ppv);\
                }\
        }\
        return E_INVALIDARG;\
}\
\
EXPORT_API DllCanUnloadNow(void)\
{\
        return S_FALSE ;\
}\
\
EXPORT_API DllRegisterServer(void)\
{\
        return S_OK;\
}\
\
EXPORT_API DllUnregisterServer(void)\
{\
        return S_OK;\
}

#define END_CLIDMAP_AND_EXPORTFUN }; \
        DEFINE_ALL_EXPORTFUN;

#endif
