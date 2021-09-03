#ifndef _Q_I_BUFFER_H_
#define _Q_I_BUFFER_H_

#include<QByteArray>
#include "QtComPtr.h"

struct QIBuffer : public QIUnknown
{
    virtual QHRESULT SetBuffer(const char* lpBuf,ulong dwSize,ulong dwStartIndex = 0) = 0;

    virtual QHRESULT setBuffer(const QByteArray& data) = 0;

    virtual QHRESULT GetBuffer(const char* lpoutData, ulong dwSize,ulong* pDwReadCount,ulong dwStartIndex = 0) = 0;

    virtual bool isEmpty() = 0;

    virtual void Append(const QByteArray& data) = 0;
    virtual void Append(const QString& data) = 0;
    virtual void Append(const char* data) = 0;
    virtual void Append(const char *str, int len) = 0;
    virtual void Append(QIBuffer* buffer) = 0;

    virtual int size() = 0;

    virtual const char* data() = 0;

    virtual QHRESULT Clone(QIBuffer** lpIBuufer) = 0;

    virtual QHRESULT AddTail(const char* lpBuf,ulong dwSize) = 0;

    virtual QHRESULT AddHead(const char* lpBuf,ulong dwSize) = 0;

    virtual void clear() = 0;

    virtual QByteArray& getByteArray() = 0;
};

QT_DEFINE_IID(QIBuffer, "qtcom.i.buffer")

QT_DEFINE_CLSID(CLSID_Buffer, "qtcom.class.buffer")

//safe buffer
QT_DEFINE_CLSID(CLSID_SafeBuffer, "qtcom.class.safebuffer")

Q_DECLARE_METATYPE(QtComPtr<QIBuffer>)

#endif
