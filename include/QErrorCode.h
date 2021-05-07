#ifndef _QERROR_CODE_H_
#define _QERROR_CODE_H_

#include<QString>
class QErrorCode
{
public:
    enum{
        Runtime = 5000,
        CppException,
        Canceled
    };

    QErrorCode(){
        code = 0;
    }

    QErrorCode(int _code){
        code = _code;
    }

    QErrorCode(int _code, const QString& what){
        code = _code;
        msg = what;
    }

    bool operator==(int _code) const{
        return code == _code;
    }

    bool operator!=(int _code) const{
        return code != _code;
    }

    operator int() const{
        return code;
    }

    QString what() {
        return msg;
    }

private:
    int code;
    QString msg;
};

#endif


