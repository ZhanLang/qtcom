#include <QCoreApplication>

#include"../../qtcom/mscombase.h"
#include "../../qtcom/mscomptr.h"
struct A{};
struct B{};


 MS_DEFINE_IID(A, "{dbe41a75-d5da-402a-aff7-cd347877ec00}");
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GUID ii = re_uuidof(IUnknown);
    GUID i23 = re_uuidof(A);

    return a.exec();
}
