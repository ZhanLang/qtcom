#include"wlcomype.h"
#include<QString>
template<typename  T>
class QWLResultBase
{
public:
 T code;
 QString msg;
};

class QWLResult: public QWLResultBase<LONG>
{
public:
    QWLResult(){
        code = -1;
    }

    QWLResult(LONG c) {
        code = c;
    }
    QWLResult(LONG c, const QString& m){
        code = c;
        msg = m;
    }

    operator bool() const{
        return ok();
    }

    bool operator==(LONG c) const{
        return code == c;
    }

    bool ok() const{
        return  code == 0;
    }

    ULONG getCode(){
        return code;
    }

    QString getMessage() const{
        return msg;
    }
};
