#ifndef __MSUUID_H
#define __MSUUID_H

#ifndef WIN32
typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID,IID;
#else
#include<guiddef.h>
#define MS_DEFINE_IID(iface, uuid_string)	struct __declspec(uuid(uuid_string)) iface
#endif

#define MS_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    const GUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }


#ifndef GUID_NULL
    MS_DEFINE_GUID(GUID_NULL,
            0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
#endif

#ifndef WIN32
    template <class T>
    const GUID & _luuidof( )
    {
        static GUID guid = GUID_NULL;
        return guid;
    }

    #define MS_DEFINE_IID(iface, uuid_string)	\
    template <class>\
    const GUID & _luuidof<iface>( )\
    {\
        static GUID guid = S2GUIDA(uuid_string);\
        return guid;\
    }

    #define __uuidof(iface) _luuidof<iface>
#endif


#include "linux/lcom.h"

#endif
