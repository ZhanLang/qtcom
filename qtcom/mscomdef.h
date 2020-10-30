#ifndef _RE_MSCOMDEF_H_
#define _RE_MSCOMDEF_H_

#ifndef WIN32
    typedef LONG HRESULT;
    #define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif

#define RFAILED(x) { HRESULT _h_r_ = (x); if(FAILED(_h_r_)) return _h_r_; }

#endif
