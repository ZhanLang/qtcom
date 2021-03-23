#ifndef __QWLTYPE_H__
#define __QWLTYPE_H__

#ifdef WIN32
#include <Windows.h>
#else
typedef long            HRESULT;
typedef long            LONG;
typedef unsigned long   ULONG;
typedef unsigned long   DWORD;
typedef const CLSID& REFCLSID,REFGUID,REFIID;
typedef int   BOOL;
typedef void* LPVOID;

typedef struct _GUID
{
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

#define IID GUID
#define CLSID GUID
static GUID GUID_NULL = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0 ,0}};

inline bool operator == (const GUID& left, const GUID& right)
{
    return memcmp(&left, &right, sizeof(GUID)) == 0;
}

inline bool operator < (const GUID& left, const GUID& right)
{
    return memcmp(&left, &right, sizeof(GUID)) < 0;
}

#define STDMETHOD(method) HRESULT __stdcall




#define FAILED(hr) (((HRESULT)(hr)) < 0)

#endif



#define RFAILED(x) { HRESULT _h_r_ = (x); if(FAILED(_h_r_)) return _h_r_; }
#define RASSERT(x, _h_r_) { if(!(x)) return _h_r_; }
#define RASSERTP(x, _h_r_) { if(NULL==(x)) return _h_r_; }

#endif //__QWLTYPE_H__
HRESULT
