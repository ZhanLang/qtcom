
#ifndef __GUIDFUN_H__
#define __GUIDFUN_H__

#include <stdio.h>
#include <string.h>
#include "comdef.h"


#define GUID_SLEN	40

#define USES_GUIDCONVERSIONW wchar lpGuidBuf[GUID_SLEN];
#define USES_GUIDCONVERSIONA char lpGuidBuf[GUID_SLEN];

#ifdef UNICODE
        #define USES_GUIDCONVERSION USES_GUIDCONVERSIONW
#else
        #define USES_GUIDCONVERSION USES_GUIDCONVERSIONA
#endif
#pragma warning( push )
//bHaveBracket是否带括号
static const char* GUIDToSA(const GUID& guid, char* lpGuidBuf, unsigned dwSize, bool bHaveBracket = true)
{
    if(bHaveBracket && !(lpGuidBuf && dwSize >= 38))
        return nullptr;
    if(!bHaveBracket && !(lpGuidBuf && dwSize >= 36))
        return nullptr;

    sprintf(lpGuidBuf,
        bHaveBracket?"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}":"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
        guid.Data1,
        guid.Data2,
        guid.Data3,
        guid.Data4[0],
        guid.Data4[1],
        guid.Data4[2],
        guid.Data4[3],
        guid.Data4[4],
        guid.Data4[5],
        guid.Data4[6],
        guid.Data4[7]);

    return lpGuidBuf;
};


#pragma warning( pop )

#define GUID2SA(guid) GUIDToSA(guid, lpGuidBuf, GUID_SLEN)
#define GUID2S_NOBracketA(guid) GUIDToSA(guid, lpGuidBuf, GUID_SLEN, false)

#ifdef UNICODE
        #define S2GUID				S2GUIDW
        #define GUIDToS				GUIDToSW
        #define GUID2S				GUID2SW
        #define GUID2S_NOBracket	GUID2S_NOBracketW
#else
        #define S2GUID				S2GUIDA
        #define GUIDToS				GUIDToSA
        #define GUID2S				GUID2SA
        #define GUID2S_NOBracket	GUID2S_NOBracketA
#endif
#endif

