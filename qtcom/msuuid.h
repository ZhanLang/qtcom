#ifndef __MSUUID_H
#define __MSUUID_H

#ifndef WIN32
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID,IID,CLSID;
#else
#include<guiddef.h>
#endif

static GUID GUID_NULL = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0 ,0}};
typedef const CLSID& REFCLSID;
#endif
