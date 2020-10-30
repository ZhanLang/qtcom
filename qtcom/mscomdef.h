#ifndef _RE_MSCOMDEF_H_
#define _RE_MSCOMDEF_H_

#ifndef WIN32

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID,IID,CLSID;

typedef long HRESULT;
typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef const CLSID& REFCLSID;

#ifdef UNICODE
    typedef const wchar_t* LPCTSTR;
#else
    typedef const char* LPCTSTR;
#endif

#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define STDMETHODCALLTYPE
#define STDMETHOD(method)        virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method)  virtual  type   STDMETHODCALLTYPE method
#define EXTERN_C extern "C"

static GUID GUID_NULL = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0 ,0}};

#endif
#define RFAILED(x) { HRESULT _h_r_ = (x); if(FAILED(_h_r_)) return _h_r_; }
#define __countof(x) (sizeof(x) / sizeof(x[0]))

#ifndef WIN32
template <class T>
inline const GUID&  _luuidof( )
{
    static GUID guid = GUID_NULL;
    return guid;
}

#define MS_DEFINE_IID(iface, uuid_string)	\
template<> \
inline const GUID& _luuidof<iface>( )\
{\
    static GUID guid = S2GUIDA(uuid_string);\
    return guid;\
}

#define __uuidof(X) _luuidof<X>()


#else
#   define MS_DEFINE_IID(iface, uuid_string)	struct __declspec(uuid(uuid_string)) iface
#endif

#define MS_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID DECLSPEC_SELECTANY name \
    = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#define re_uuidof(iface)	__uuidof(iface)
#endif
