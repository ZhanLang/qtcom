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

#endif
