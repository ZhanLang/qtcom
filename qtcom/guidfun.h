
#ifndef __GUIDFUN_H__
#define __GUIDFUN_H__

#include <stdio.h>
#include <string.h>
#include "msuuid.h"
#include "mscomdef.h"


#define GUID_SLEN	40

#define USES_GUIDCONVERSIONW WCHAR lpGuidBuf[GUID_SLEN];
#define USES_GUIDCONVERSIONA CHAR lpGuidBuf[GUID_SLEN];

#ifdef UNICODE
        #define USES_GUIDCONVERSION USES_GUIDCONVERSIONW
#else
        #define USES_GUIDCONVERSION USES_GUIDCONVERSIONA
#endif
#pragma warning( push )
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

