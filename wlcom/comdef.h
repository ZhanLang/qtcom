#ifndef _RE_MSCOMDEF_H_
#define _RE_MSCOMDEF_H_
#include <stdio.h>
#include <string.h>
#ifndef WIN32

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

#define IID GUID
#define CLSID GUID

typedef long HRESULT;
typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef const CLSID& REFCLSID,REFGUID,REFIID;
typedef int   BOOL;
typedef void* LPVOID;
#ifdef UNICODE
    typedef const wchar_t* LPCTSTR;
#else
    typedef const char* LPCTSTR;
#endif

#define S_OK            0
#define S_FALSE         ((HRESULT)1L)
#define E_NOINTERFACE   0x80004002L
#define E_UNEXPECTED    0x8000FFFFL
#define E_NOTIMPL       0x80004001L
#define E_INVALIDARG    0x80070057L

#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define STDMETHODCALLTYPE
#define STDMETHOD(method)        virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method)  virtual  type   STDMETHODCALLTYPE method
#define EXTERN_C extern "C"

static GUID GUID_NULL = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0 ,0}};

inline bool operator == (const GUID& left, const GUID& right)
{
    return memcmp(&left, &right, sizeof(GUID)) == 0;
}
#else
#include <Windows.h>
#endif
#define RFAILED(x) { HRESULT _h_r_ = (x); if(FAILED(_h_r_)) return _h_r_; }
#define RASSERT(x, _h_r_) { if(!(x)) return _h_r_; }
#define __countof(x) (sizeof(x) / sizeof(x[0]))

static GUID S2GUIDA(const char* lpString, bool bHaveBracket = true)
{
    GUID guid = GUID_NULL;
    //todo it's lpString shit thing
    if(
        (bHaveBracket && strlen(lpString)!=38)
        || (!bHaveBracket && strlen(lpString)!=36))
        return guid;

    char szTemp[128]={0};
    strcpy(szTemp , lpString);

    long  Data234[ 10 ] = {0};
    sscanf(szTemp,
        bHaveBracket?"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}":"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
       &guid.Data1,
       Data234,
       Data234+1,
       Data234+2,
       Data234+3,
       Data234+4,
       Data234+5,
       Data234+6,
       Data234+7,
       Data234+8,
       Data234+9);

    guid.Data2 = (unsigned short)Data234[0];
    guid.Data3 = (unsigned short)Data234[1];
    guid.Data4[0] = (unsigned char)Data234[2];
    guid.Data4[1] = (unsigned char)Data234[3];
    guid.Data4[2] = (unsigned char)Data234[4];
    guid.Data4[3] = (unsigned char)Data234[5];
    guid.Data4[4] = (unsigned char)Data234[6];
    guid.Data4[5] = (unsigned char)Data234[7];
    guid.Data4[6] = (unsigned char)Data234[8];
    guid.Data4[7] = (unsigned char)Data234[9];

    return guid;
};
inline bool operator < (const GUID& left, const GUID& right)
{
    return memcmp(&left, &right, sizeof(GUID)) < 0;
}


#ifndef WIN32
template <class T>
inline const GUID& _luuidof( )
{
    return GUID_NULL;
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
    EXTERN_C const GUID  name \
    = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#define re_uuidof(iface)	__uuidof(iface)


#endif
