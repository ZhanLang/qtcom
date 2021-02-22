
#ifndef __QWLTYPE_H__
#define __QWLTYPE_H__
#include"qwltype.h"


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
#endif
