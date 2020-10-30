
#ifndef _RE_MSCOMPTR_H_
#define _RE_MSCOMPTR_H_

namespace mscom {;

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
 
template<class _Ptr, class _Traits = default_sentry>
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



template<class I>
struct com_ptr : sentry<I*, co_interface_sentry>
{
	typedef sentry<I*, co_interface_sentry> base;
	using base::m_p; // gcc 3.4

	// default construct:
	com_ptr() : base() {}
	// construct with:
	template<class U>
		com_ptr(const com_ptr<U>& rhs) : base() { if(rhs.m_p && FAILED(rhs.m_p->QueryInterface(re_uuidof(I), (void**)&m_p))) m_p = 0; } // gcc

	com_ptr(const com_ptr& rhs) : base() { if(rhs.m_p && FAILED(rhs.m_p->QueryInterface(re_uuidof(I), (void**)&m_p))) m_p = 0; } // gcc
	template<class U>
		com_ptr(const sentry<U*, co_interface_sentry>& rhs) : base() { if(rhs.m_p && FAILED(rhs.m_p->QueryInterface(re_uuidof(I), (void**)&m_p))) m_p = 0; } // gcc

	template<class U>
		com_ptr(U *p) : base() { if(p && FAILED(p->QueryInterface(re_uuidof(I), (void**)&m_p))) m_p = 0; }
	// operator =:
	template<class U>
		com_ptr& operator = (const com_ptr<U>& rhs) { if((void*)m_p != (void*)rhs.m_p) *this = rhs.m_p; return *this; }

	com_ptr& operator = (const com_ptr& rhs) { if(m_p != rhs.m_p) *this = rhs.m_p; return *this; }
	template<class U>
		com_ptr& operator = (const sentry<U*, co_interface_sentry>& rhs) { if((void*)m_p != (void*)rhs.m_p) *this = rhs.m_p; return *this; }

	template<class U>
		com_ptr& operator = (U *p) { if((void*)m_p == (void*)p) return *this; base::operator=(0); if(p && FAILED(p->QueryInterface(re_uuidof(I), (void**)&m_p))) m_p = 0; return *this; }
};

} //namespace mscom


#endif // duplicate inclusion protection
