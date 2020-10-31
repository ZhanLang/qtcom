#include <QCoreApplication>

#include"../../qtcom/combase.h"
#include"../../qtcom/comiface.h"
#include "../../qtcom/comptr.h"
#include"../../qtcom/comstd.h"
#include"../../qtcom/clsexport.h"
#include"../../qtcom/clsfactory.h"
#include"../../qtcom/guidfun.h"

#include<QDebug>


#include<vector>
struct ISampleInterface : public IMSBase{
    virtual int Print() = 0;
};
 MS_DEFINE_IID(ISampleInterface, "{dbe41a75-d5da-402a-aff7-cd347877ec01}");
class CSampleClass : public ISampleInterface, private CUnknownImp
{
public:
    UNKNOWN_IMP1(ISampleInterface)
    HRESULT init_class(IMSBase* in, IMSBase* punkOuter)
    {
        return S_OK;
    }
    ~CSampleClass()
    {
        Print();
    }

    virtual int Print(){
        return 99;
    }
};

MS_DEFINE_GUID(CLSID_CSampleClass,
    0x2fc8af14, 0xe405, 0x4a74, 0xb1, 0x76, 0x0, 0x3, 0x95, 0x5b, 0x11, 0xb0);

std::vector<int> bbb;



BEGIN_CLIDMAP
CLIDMAPENTRY_BEGIN
CLIDMAPENTRY(CLSID_CSampleClass, CSampleClass)
CLIDMAPENTRY_END

END_CLIDMAP
//DEFINE_ALL_EXPORTFUN

HRESULT DllGetClassObject(REFCLSID rclsid,  REFIID riid,  LPVOID* ppv)
{
    for(size_t i = 0; i < g_mapClassObject.size(); i++)
    {
        GUID g = g_mapClassObject[i].clsid;
        USES_GUIDCONVERSION
        GUIDToSA(g, lpGuidBuf, GUID_SLEN);
        std::string s; s+= lpGuidBuf;

        qDebug() << lpGuidBuf;
        GUIDToSA(rclsid, lpGuidBuf, GUID_SLEN);
        std::string s1 ;s1+=( lpGuidBuf);
qDebug() << lpGuidBuf;
qDebug() << "aaaaaaaaaaaaaaaaaaa";
        if( rclsid == g )
        {
                return g_mapClassObject[i].pfnGetClassObject(riid, ppv);
        }
    }
    return E_INVALIDARG;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

bbb.push_back(0);
   com_ptr<ISampleInterface> smp;
   com_ptr<IMSClassFactory> fac;
   DllGetClassObject(CLSID_CSampleClass, re_uuidof(IMSClassFactory), (void**)&fac.m_p);
   if(fac)
        fac->CreateInstance(INULL, INULL, re_uuidof(ISampleInterface), (void**)&smp.m_p);

   int i = g_mapClassObject.size();
   //int n = __countof(g_mapClassObject);
   //GUID G = g_mapClassObject[0].clsid;
    smp->Print();
    smp = INULL;

    return a.exec();
}
