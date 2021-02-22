#ifndef __COM_STD_H__
#define __COM_STD_H__
#define MAX_PROGIDLEN	100

#include<atomic>
#include <string.h>
#include "qwluuid.h"
#include "qwlinterface.h"
#include "qwlcomptr.h"

class CUnknownImp
{
public:
    std::atomic<ULONG> m_RefCount;
    CUnknownImp(): m_RefCount(0) {}
    virtual ~CUnknownImp(){}
};





#define QIBEGIN	\
        STDMETHOD(QueryInterface)(const IID&  riid, void **ppv) {

#define QIUNKNOWN	\
    if(re_uuidof(IWLComBase) == riid) { *ppv = static_cast<IWLComBase*>(this); AddRef(); return S_OK; }

#define QIUNKNOWN_(icast)	\
    if(re_uuidof(IWLComBase) == riid) { *ppv = static_cast<IWLComBase*>(static_cast<icast*>(this)); AddRef(); return S_OK; }

#define QIENTRY(iface)	\
	if(re_uuidof(iface) == riid) { *ppv = static_cast<iface*>(this); AddRef(); return S_OK; }

#define QIENTRY_(iface, icast)	\
	if(re_uuidof(iface) == riid) { *ppv = static_cast<iface*>(static_cast<icast*>(this)); AddRef(); return S_OK; }

#define QIEND \
	return E_NOINTERFACE; }

// 我要看对象计数
#ifdef _DEBUG_INTERLOCKED_VIEW
class CInterlockedView_
{
public:
	ULONG& m_RefCount;
	void* m_this;
	CInterlockedView_(void* pthis, ULONG& RefCount):m_this(pthis),m_RefCount(RefCount)
	{
		TCHAR out[64] = {0};
		wsprintf(out, _T("Enter %x = %d ------> "), m_this, m_RefCount);
		OutputDebugString(out);
	}
	~CInterlockedView_()
	{
		TCHAR out[64] = {0};
		wsprintf(out, _T("Leave %x = %d\n"), m_this, m_RefCount);
		OutputDebugString(out);
	}
};
#define ADDREF	\
	STDMETHOD_(ULONG, AddRef)()	\
	{ CInterlockedView_ v(this, m_RefCount); return InterlockedIncrement((LONG*)(LPVOID)&m_RefCount); }

#define RELEASE	\
	STDMETHOD_(ULONG, Release)()	\
	{ CInterlockedView_ v(this, m_RefCount); if(InterlockedDecrement((LONG*)(LPVOID)&m_RefCount)) return m_RefCount; delete this; return 0; }

#else


#define ADDREF	\
    STDMETHOD_(ULONG, AddRef)()	\
    {return this->m_RefCount++; }

#define RELEASE	\
    STDMETHOD_(ULONG, Release)()	\
       { this->m_RefCount--; if(this->m_RefCount) return this->m_RefCount; delete this; return 0; }

#endif

#define UNKNOWN_IMP_SPEC(entrys) \
	QIBEGIN  QIUNKNOWN  entrys  QIEND  ADDREF  RELEASE

#define UNKNOWN_IMP_SPEC_(icast, entrys) \
	QIBEGIN  QIUNKNOWN_(icast)  entrys  QIEND  ADDREF  RELEASE


#define ADDREF_EX	\
	STDMETHOD_(ULONG, AddRef)()	\
{return InterlockedIncrement((LONG*)(LPVOID)&m_RefCount); }

#define RELEASE_EX	\
	STDMETHOD_(ULONG, Release)()	\
{ if(InterlockedDecrement((LONG*)(LPVOID)&m_RefCount)) return m_RefCount; /*delete this;*/ return 0; }

#define UNKNOWN_IMP_SPEC_EX(entrys) \
	QIBEGIN  QIUNKNOWN  entrys  QIEND  ADDREF_EX  RELEASE_EX

#define UNKNOWN_IMP1(i) \
	UNKNOWN_IMP_SPEC( QIENTRY(i) )
#define UNKNOWN_IMP2(i1, i2) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_IMP3(i1, i2, i3) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_IMP4(i1, i2, i3, i4) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_IMP5(i1, i2, i3, i4, i5) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_IMP6(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_IMP7(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )
#define UNKNOWN_IMP8(i1, i2, i3, i4, i5, i6, i7, i8) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) QIENTRY(i8) )
#define UNKNOWN_IMP9(i1, i2, i3, i4, i5, i6, i7, i8, i9) \
	UNKNOWN_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) QIENTRY(i8) QIENTRY(i9))


#define UNKNOWN_IMP1_(i) \
	UNKNOWN_IMP_SPEC_(i,  QIENTRY(i))
#define UNKNOWN_IMP2_(i1, i2) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_IMP3_(i1, i2, i3) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_IMP4_(i1, i2, i3, i4) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_IMP5_(i1, i2, i3, i4, i5) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_IMP6_(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_IMP7_(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )
#define UNKNOWN_IMP8_(i1, i2, i3, i4, i5, i6, i7, i8) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) QIENTRY(i8) )
#define UNKNOWN_IMP9_(i1, i2, i3, i4, i5, i6, i7, i8, i9) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) QIENTRY(i8) QIENTRY(i9))
	
#define UNKNOWN_IMP10_(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) QIENTRY(i8) QIENTRY(i9) QIENTRY(i10))

#define UNKNOWN_IMP11_(i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11) \
	UNKNOWN_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) QIENTRY(i8) QIENTRY(i9) QIENTRY(i10) QIENTRY(i11))

//////////////////////////////////////////////////////////////////////////


class IUnknown_Nondelegate
{
public:
	STDMETHOD(QueryInterface_Nondelegate)(REFGUID riid, void **ppv) = 0;
	STDMETHOD_(ULONG, AddRef_Nondelegate)() = 0;
	STDMETHOD_(ULONG, Release_Nondelegate)() = 0;
};

struct INoRefCom : public IUnknown
{
	STDMETHOD_(ULONG,AddRef)(void){return 1;}
	STDMETHOD_(ULONG,Release)(void){return 1;}
	STDMETHOD(QueryInterface)(const IID & Want_IID,void **ppObject){return E_NOTIMPL;}
};
QWL_DEFINE_IID(INoRefCom,"{FD00FF7A-1429-4310-80DB-BF33C04211BE}");


class QUnknownImp_Inner
{
public:
	ULONG m_RefCount;
    QUnknownImp_Inner(): m_RefCount(0), m_punkOuter(0) {}
public:
    IWLComBase *m_punkOuter;
    HRESULT init_class_inner(IWLComBase *punkOuter)
	{
		m_punkOuter = punkOuter;
		return S_OK;
	}
};


#define QIBEGIN_NONDELEGATE	\
	STDMETHOD(QueryInterface_Nondelegate)(REFGUID riid, void **ppv) {

#define QIUNKNOWN_NONDELEGATE	\
    if(re_uuidof(IWLComBase) == riid) { *ppv = static_cast<IUnknown_Nondelegate*>(this); AddRef_Nondelegate(); return S_OK; }

#define QIUNKNOWN_NONDELEGATE_(icast)	\
    if(re_uuidof(IWLComBase) == riid) { *ppv = static_cast<IUnknown_Nondelegate*>(static_cast<icast*>(this)); AddRef_Nondelegate(); return S_OK; }

#define ADDREF_NONDELEGATE	\
	STDMETHOD_(ULONG, AddRef_Nondelegate)()	\
	{ return InterlockedIncrement((LONG*)(LPVOID)&m_RefCount); }

#define RELEASE_NONDELEGATE	\
	STDMETHOD_(ULONG, Release_Nondelegate)()	\
	{ if(InterlockedDecrement((LONG*)(LPVOID)&m_RefCount)) return m_RefCount; delete this ; return 0; }

#define QI_INNER	\
	STDMETHOD(QueryInterface)(REFGUID riid, void **ppv)	\
	{ if(m_punkOuter) { return m_punkOuter->QueryInterface(riid, ppv); } else { return QueryInterface_Nondelegate(riid, ppv); } }

#define ADDREF_INNER	\
	STDMETHOD_(ULONG, AddRef)()	\
	{ if(m_punkOuter) { return m_punkOuter->AddRef(); } else { return AddRef_Nondelegate(); } }

#define RELEASE_INNER	\
	STDMETHOD_(ULONG, Release)()	\
	{ if(m_punkOuter) { return m_punkOuter->Release(); } else { return Release_Nondelegate(); } }

#define UNKNOWN_INNER_IMP_SPEC(entrys) \
	QIBEGIN_NONDELEGATE  QIUNKNOWN_NONDELEGATE  entrys  QIEND  ADDREF_NONDELEGATE  RELEASE_NONDELEGATE  QI_INNER  ADDREF_INNER  RELEASE_INNER

#define UNKNOWN_INNER_IMP_SPEC_(icast, entrys) \
	QIBEGIN_NONDELEGATE  QIUNKNOWN_NONDELEGATE_(icast)  entrys  QIEND  ADDREF_NONDELEGATE  RELEASE_NONDELEGATE  QI_INNER  ADDREF_INNER  RELEASE_INNER

#define UNKNOWN_INNER_IMP1(i) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i) )
#define UNKNOWN_INNER_IMP2(i1, i2) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_INNER_IMP3(i1, i2, i3) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_INNER_IMP4(i1, i2, i3, i4) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_INNER_IMP5(i1, i2, i3, i4, i5) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_INNER_IMP6(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_INNER_IMP7(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_INNER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )

#define UNKNOWN_INNER_IMP2_(i1, i2) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_INNER_IMP3_(i1, i2, i3) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_INNER_IMP4_(i1, i2, i3, i4) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_INNER_IMP5_(i1, i2, i3, i4, i5) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_INNER_IMP6_(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_INNER_IMP7_(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_INNER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )




#define QIEND_OUTER \
	return m_punkInner->QueryInterface(riid, ppv); }

#define UNKNOWN_OUTER_IMP_SPEC(entrys) \
	QIBEGIN  QIUNKNOWN  entrys  QIEND_OUTER  ADDREF  RELEASE

#define UNKNOWN_OUTER_IMP_SPEC_(icast, entrys) \
	QIBEGIN  QIUNKNOWN_(icast)  entrys  QIEND_OUTER  ADDREF  RELEASE

#define UNKNOWN_OUTER_IMP0 \
	QIBEGIN  QIUNKNOWN  QIEND_OUTER  ADDREF  RELEASE
#define UNKNOWN_OUTER_IMP1(i) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i) )
#define UNKNOWN_OUTER_IMP2(i1, i2) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_OUTER_IMP3(i1, i2, i3) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_OUTER_IMP4(i1, i2, i3, i4) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_OUTER_IMP5(i1, i2, i3, i4, i5) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_OUTER_IMP6(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_OUTER_IMP7(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_OUTER_IMP_SPEC( QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )
	
#define UNKNOWN_OUTER_IMP2_(i1, i2) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) )
#define UNKNOWN_OUTER_IMP3_(i1, i2, i3) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) )
#define UNKNOWN_OUTER_IMP4_(i1, i2, i3, i4) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) )
#define UNKNOWN_OUTER_IMP5_(i1, i2, i3, i4, i5) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) )
#define UNKNOWN_OUTER_IMP6_(i1, i2, i3, i4, i5, i6) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) )
#define UNKNOWN_OUTER_IMP7_(i1, i2, i3, i4, i5, i6, i7) \
	UNKNOWN_OUTER_IMP_SPEC_(i1, QIENTRY(i1) QIENTRY(i2) QIENTRY(i3) QIENTRY(i4) QIENTRY(i5) QIENTRY(i6) QIENTRY(i7) )
	

class QNullObjcetUnkown :public IWLComBase, private CUnknownImp
{
public:

    UNKNOWN_IMP1(IWLComBase);
    // std factory invoke:
    STDMETHOD(init_class)(IWLComBase* /*prot*/, IWLComBase* punkOuter)
    {
        return !punkOuter ? S_OK : E_INVALIDARG;
    }

    STDMETHOD(init_class_inner)(IWLComBase* punkOuter)
    {
        return !punkOuter ? S_OK : E_INVALIDARG;
    }
};


#endif // duplicate inclusion protection
