#ifndef _RE_MSCOMDEF_H_
#define _RE_MSCOMDEF_H_

#ifndef WIN32
    typedef long HRESULT;
    typedef  long LONG;
    typedef unsigned long ULONG;
typedef   unsigned long DWORD;


    #ifdef UNICODE
        typedef const wchar_t* LPCTSTR;
    #else
        typedef const char* LPCTSTR;
    #endif
    #define FAILED(hr) (((HRESULT)(hr)) < 0)
    #define  STDMETHODCALLTYPE
    #define  STDMETHOD(method)        virtual HRESULT STDMETHODCALLTYPE method
    #define  STDMETHOD_(type,method)  virtual  type   STDMETHODCALLTYPE method
    #define  EXTERN_C extern "C"

#endif
#define RFAILED(x) { HRESULT _h_r_ = (x); if(FAILED(_h_r_)) return _h_r_; }

#define __countof(x) (sizeof(x) / sizeof(x[0]))


#endif
