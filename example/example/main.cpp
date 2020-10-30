#include <QCoreApplication>

#include"../../qtcom/msuuid.h"
struct A{};
struct B{};
 MS_DEFINE_IID(A, "dbe41a75-d5da-402a-aff7-cd347877ec00");
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    IID i1 = __uuidof(A);
   // IID i2 = _luuidof<B>();
    return a.exec();
}
