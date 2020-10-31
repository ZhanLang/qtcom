/*******************************************************************************
* com的基接口IUnknown
*
*******************************************************************************/
#ifndef _IMSBASE_H
#define _IMSBASE_H
#include <stdio.h>
#include"comdef.h"
#include"guidfun.h"



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
