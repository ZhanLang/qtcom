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
#ifndef WIN32
struct IUnknown
{
    virtual ULONG   STDMETHODCALLTYPE AddRef( void) = 0;
    virtual ULONG   STDMETHODCALLTYPE Release( void) = 0;
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void  **ppvObject) = 0;
};
MS_DEFINE_IID(IUnknown,"{00000000-0000-0000-C000-000000000046}");
#endif

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
