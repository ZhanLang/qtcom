#ifndef QUNKNWNIMPL_H
#define QUNKNWNIMPL_H

#include<QAtomicInteger>
#include"qunknwn.h"
class CUnknownImp
{
public:
    QAtomicInteger<ulong> m_RefCount;
    CUnknownImp(): m_RefCount(0) {}
    virtual ~CUnknownImp(){}
};

#define QIBEGIN	\
        virtual QHRESULT STDMETHODCALL QueryInterface(const IID&  riid, void **ppv) {

#define QIUNKNOWN	\
    if(qt_uuidof(QIUnknown) == riid) { *ppv = static_cast<QIUnknown*>(this); AddRef(); return QS_OK; }

#define QIUNKNOWN_(icast)	\
    if(qt_uuidof(QIUnknown) == riid) { *ppv = static_cast<QIUnknown*>(static_cast<icast*>(this)); AddRef(); return QS_OK; }

#define QIENTRY(iface)	\
    if(qt_uuidof(iface) == riid) { *ppv = static_cast<iface*>(this); AddRef(); return QS_OK; }

#define QIENTRY_(iface, icast)	\
    if(qt_uuidof(iface) == riid) { *ppv = static_cast<iface*>(static_cast<icast*>(this)); AddRef(); return QS_OK; }

#define QIEND \
    return QE_NOINTERFACE; }

#define ADDREF	\
    virtual ulong STDMETHODCALL AddRef()	\
    {return this->m_RefCount++; }

#define RELEASE	\
    virtual ulong STDMETHODCALL Release()	\
       { this->m_RefCount--; if(this->m_RefCount) return this->m_RefCount; delete this; return 0; }


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

//////////////////////////////////////////////////////////////////////////


struct QIUnknown_Nondelegate
{
public:
    virtual QHRESULT STDMETHODCALL QueryInterface_Nondelegate(const QUuid& riid, void **ppv) = 0;
    virtual int STDMETHODCALL AddRef_Nondelegate() = 0;
    virtual int STDMETHODCALL Release_Nondelegate() = 0;
};

struct QINoRefCom : public QIUnknown
{
    virtual ulong STDMETHODCALL AddRef( ){return 1;}
    virtual ulong STDMETHODCALL Release(){return 1;}
    virtual QHRESULT STDMETHODCALL QueryInterface(const QUuid & Want_IID,void **ppObject){return 1;}
};
QT_DEFINE_IID(QINoRefCom,"{FD00FF7A-1429-4310-80DB-BF33C04211BE}");


class QUnknownImp_Inner
{
public:
    ulong m_RefCount;
    QUnknownImp_Inner(): m_RefCount(0), m_punkOuter(0) {}
public:
    QIUnknown *m_punkOuter;
    int init_class_inner(QIUnknown *punkOuter)
    {
        m_punkOuter = punkOuter;
        return 0;
    }
};


#define QIBEGIN_NONDELEGATE	\
    STDMETHOD(QueryInterface_Nondelegate)(REFGUID riid, void **ppv) {

#define QIUNKNOWN_NONDELEGATE	\
    if(qt_uuidof(QIUnknown) == riid) { *ppv = static_cast<IUnknown_Nondelegate*>(this); AddRef_Nondelegate(); return QS_OK; }

#define QIUNKNOWN_NONDELEGATE_(icast)	\
    if(qt_uuidof(QIUnknown) == riid) { *ppv = static_cast<IUnknown_Nondelegate*>(static_cast<icast*>(this)); AddRef_Nondelegate(); return QS_OK; }

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


#endif // QUNKNWNIMPL_H
