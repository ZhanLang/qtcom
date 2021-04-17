#ifndef QCOMPTR_H
#define QCOMPTR_H


struct default_sentry
{
    static void* default_value() { return 0; }
    template<class _Ptr> static bool equal_to(_Ptr l, _Ptr r) { return l == r; }
    template<class _Ptr> static void destroy(_Ptr p) { delete p; }
};

struct default_array_sentry
{
    static void* default_value() { return 0; }
    template<class _Ptr> static bool equal_to(_Ptr l, _Ptr r) { return l == r; }
    template<class _Ptr> static void destroy(_Ptr p) { delete [] p; }
};

struct co_interface_sentry
{
    static void* default_value() { return 0; }
    template<class _Ptr> static bool equal_to(_Ptr, _Ptr) { return false; }
    template<class _Ptr> static void destroy(_Ptr p) { if(p) p->Release(); }
};

struct handle_sentry
{
    static void* default_value() { return 0; }
    template<class _Ptr> static bool equal_to(_Ptr l, _Ptr r) { return l == r; }
    template<class _Ptr> static void destroy(_Ptr p) { if(p)CloseHandle(p); }
};

template<class _Ptr,
         class _Traits = default_sentry>
struct sentry
{
public:
    sentry(const _Traits& tr = _Traits()) : m_tr(tr) { m_p = (_Ptr)m_tr.default_value(); }
    sentry(_Ptr p, const _Traits& tr = _Traits()) : m_p(p), m_tr(tr) {}
    ~sentry() { m_tr.destroy(m_p); }
    sentry& operator = (_Ptr p) { if(!m_tr.equal_to(m_p, p)) { m_tr.destroy(m_p); m_p = p; } return *this; }
    _Ptr detach() { _Ptr tmp = m_p; m_p = (_Ptr)m_tr.default_value(); return tmp; }
    operator _Ptr () const { return m_p; }
    _Ptr operator -> () const { return m_p; }
    void reset(){m_p = (_Ptr)m_tr.default_value();}
//	_Ptr* operator & () { return &m_p; }
public:
    _Ptr m_p;
    _Traits m_tr;
private:
    sentry(const sentry&);
    sentry& operator = (const sentry&);
};


#ifndef IS_COMPILER_VC6
    #ifdef WIN32
        #if(_MSC_VER < 1300)
            #define IS_COMPILER_VC6 1
        #else
            #define IS_COMPILER_VC6 0
        #endif
    #else
        #define IS_COMPILER_VC6 0
    #endif
#endif



//com智能指针
template<class I>
struct QtComPtr : sentry<I*, co_interface_sentry>
{
    typedef sentry<I*, co_interface_sentry> base;
    using base::m_p; // gcc 3.4
    // default construct:
    QtComPtr() : base() {}
    // construct with:
    template<class U>
        QtComPtr(const QtComPtr<U>& rhs) : base() { if(rhs.m_p && QFAILED(rhs.m_p->QueryInterface(qt_uuidof(I), (void**)&m_p))) m_p = 0; } // gcc
#if(!IS_COMPILER_VC6)
    QtComPtr(const QtComPtr& rhs) : base() { if(rhs.m_p && QFAILED(rhs.m_p->QueryInterface(qt_uuidof(I), (void**)&m_p))) m_p = 0; } // gcc
    template<class U>
        QtComPtr(const sentry<U*, co_interface_sentry>& rhs) : base() { if(rhs.m_p && QFAILED(rhs.m_p->QueryInterface(qt_uuidof(I), (void**)&m_p))) m_p = 0; } // gcc
#endif
    template<class U>
        QtComPtr(U *p) : base() { if(p && QFAILED(p->QueryInterface(qt_uuidof(I), (void**)&m_p))) m_p = 0; }
    // operator =:
    template<class U>
        QtComPtr& operator = (const QtComPtr<U>& rhs) { if((void*)m_p != (void*)rhs.m_p) *this = rhs.m_p; return *this; }
#if(!IS_COMPILER_VC6)
    QtComPtr& operator = (const QtComPtr& rhs) { if(m_p != rhs.m_p) *this = rhs.m_p; return *this; }
    template<class U>
        QtComPtr& operator = (const sentry<U*, co_interface_sentry>& rhs) { if((void*)m_p != (void*)rhs.m_p) *this = rhs.m_p; return *this; }
#endif
    template<class U>
        QtComPtr& operator = (U *p) { if((void*)m_p == (void*)p) return *this; base::operator=(0); if(p && QFAILED(p->QueryInterface(qt_uuidof(I), (void**)&m_p))) m_p = 0; return *this; }
};

#endif
