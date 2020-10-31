#include <QCoreApplication>

#include"../../qtcom/combase.h"
#include"../../qtcom/comiface.h"
#include "../../qtcom/comptr.h"
#include"../../qtcom/comstd.h"
#include"../../qtcom/clsfactory.h"
#include"../../qtcom/clsexport.h"

struct ISampleInterface : public IMSBase{
    virtual int Print() = 0;
};
 MS_DEFINE_IID(ISampleInterface, "{dbe41a75-d5da-402a-aff7-cd347877ec01}");
class CSampleClass : public ISampleInterface, private CUnknownImp
{
public:
    HRESULT init_class(IMSBase* in, IMSBase* punkOuter)
    {
        return S_OK;
    }
    ~CSampleClass()
    {
        Print();
    }
    UNKNOWN_IMP1(ISampleInterface);
    virtual int Print(){
        return 99;
    }
};

MS_DEFINE_GUID(CLSID_CSampleClass,
    0x2fc8af14, 0xe405, 0x4a74, 0xb1, 0x76, 0x0, 0x3, 0x95, 0x5b, 0x11, 0xb0);

BEGIN_CLIDMAP
CLIDMAPENTRY(CLSID_CSampleClass, CSampleClass)
END_CLIDMAP
DEFINE_ALL_EXPORTFUN

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

   com_ptr<ISampleInterface> smp;
   com_ptr<IMSClassFactory> fac;
   DllGetClassObject(CLSID_CSampleClass, re_uuidof(IMSClassFactory), (void**)&fac.m_p);
   if(fac)
        fac->CreateInstance(INULL, INULL, re_uuidof(ISampleInterface), (void**)&smp.m_p);

    smp->Print();
    smp = INULL;

    return a.exec();
}
