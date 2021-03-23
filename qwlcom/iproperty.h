#ifndef IPROPERTY_H
#define IPROPERTY_H

#include<IUnknown>

//////////////////////////////////////////////////////////////////////////
// Property:
struct IProperty : public IUnknown
{
    typedef GUID KeyType;

    STDMETHOD(SetProperty)(const GUID& rpid, const PROPVARIANT *pv) = 0;
    STDMETHOD(GetProperty)(const GUID& rpid, const PROPVARIANT **ppv) = 0;
    STDMETHOD(DeleteProperty)(const GUID& rpid) = 0;
    STDMETHOD(Clear)() = 0;
};
QWL_DEFINE_IID(IProperty, "{39AA125E-D7F1-46b1-82EB-6D8527269279}");

// {ED20E0E5-2357-4825-B3FA-198AEC674E81}
QWL_DEFINE_GUID(CLSID_QProperty,
               0xed20e0e5, 0x2357, 0x4825, 0xb3, 0xfa, 0x19, 0x8a, 0xec, 0x67, 0x4e, 0x81);

// {AD4F3A47-0CD6-43de-BC22-E8BE24FFD424}
QWL_DEFINE_GUID(CLSID_QProperty_thread,
               0xad4f3a47, 0xcd6, 0x43de, 0xbc, 0x22, 0xe8, 0xbe, 0x24, 0xff, 0xd4, 0x24);

//-------------------------------------------------------------------------------
struct IProperty2 : public IUnknown
{
    typedef DWORD KeyType;

    STDMETHOD(SetProperty)(const DWORD& rpid, const PROPVARIANT *pv) = 0;
    STDMETHOD(GetProperty)(const DWORD& rpid, const PROPVARIANT **ppv) = 0;
    STDMETHOD(DeleteProperty)(const DWORD& rpid) = 0;
    STDMETHOD(Clear)() = 0;
};
QWL_DEFINE_IID(IProperty2, "{21C975F4-EEE1-4c90-8D39-7D94A6AEDAB7}");

// {2100E98D-B13E-4306-8081-50F325B10586}
QWL_DEFINE_GUID(CLSID_QProperty2,
               0x2100e98d, 0xb13e, 0x4306, 0x80, 0x81, 0x50, 0xf3, 0x25, 0xb1, 0x5, 0x86);

// {0AEF80FB-9BAF-4e66-96B3-784ED0FCECF1}
QWL_DEFINE_GUID(CLSID_QProperty2_thread,
               0xaef80fb, 0x9baf, 0x4e66, 0x96, 0xb3, 0x78, 0x4e, 0xd0, 0xfc, 0xec, 0xf1);


//----------------------------------------------------------------------------------
struct IPropertyStr : public IUnknown
{
    STDMETHOD(SetProperty)( LPCSTR rpid, const PROPVARIANT *pv) = 0;
    STDMETHOD(GetProperty)( LPCSTR rpid, const PROPVARIANT **ppv) = 0;
    STDMETHOD(DeleteProperty)(LPCSTR rpid) = 0;
    STDMETHOD(Clear)() = 0;
};
QWL_DEFINE_IID(IPropertyStr, "{1187FFD7-5BAD-405B-9667-9BF093248E0B}");


// {C152A044-68CC-4F6D-AC27-3BC4E34593C3}
QWL_DEFINE_GUID(CLSID_QPropertyStr,
    0xc152a044, 0x68cc, 0x4f6d, 0xac, 0x27, 0x3b, 0xc4, 0xe3, 0x45, 0x93, 0xc3);


// {BA204A27-A494-4EBA-A5FA-80B87D3CEEF8}
QWL_DEFINE_GUID(CLSID_QPropertyStr_thread,
    0xba204a27, 0xa494, 0x4eba, 0xa5, 0xfa, 0x80, 0xb8, 0x7d, 0x3c, 0xee, 0xf8);


#endif // IPROPERTY_H
