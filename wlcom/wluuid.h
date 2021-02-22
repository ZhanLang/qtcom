#include"wltype.h"
#include"wlguidfn.h"
#ifdef WIN32
#define MS_DEFINE_IID(iface, uuid_string)	struct __declspec(uuid(uuid_string)) iface
#else
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
#endif

#define MS_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID  name \
    = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#define re_uuidof(iface)	__uuidof(iface)
static GUID GUID_NULL = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0 ,0}};
