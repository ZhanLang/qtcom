/*******************************************************************************
* com的基接口IUnknown
*
*******************************************************************************/
#ifndef _IMSBASE_H
#define _IMSBASE_H

#include"msuuid.h"
#include"mscomdef.h"
#include"guidfun.h"
#include <stdio.h>

#define MS_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#ifndef WIN32
template <class T>
inline const GUID&  _luuidof( )
{
    static GUID guid = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0 ,0}};
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

struct IUnknown
{
    virtual ULONG   STDMETHODCALLTYPE AddRef( void) = 0;
    virtual ULONG   STDMETHODCALLTYPE Release( void) = 0;
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void  **ppvObject) = 0;
};
MS_DEFINE_IID(IUnknown,"{00000000-0000-0000-C000-000000000046}");


#else
#define MS_DEFINE_IID(iface, uuid_string)	struct __declspec(uuid(uuid_string)) iface
#endif

#define re_uuidof(iface)	__uuidof(iface)

#define IMSBase IUnknown
#define INULL	((IUnknown*)0)
template<class T>
struct memless
{
	bool operator() (const T&left, const T&right) const
	{
		return memcmp(&left, &right, sizeof(T)) < 0;
	}
};

template<class T>
struct strless
{
	bool operator() (const T& left, const T& right) const
	{
		return left.compare(right) < 0;
	}
};
#endif // _IMSBASE_H
