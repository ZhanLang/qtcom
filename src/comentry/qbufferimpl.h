#ifndef QBUFFERIMPL_H
#define QBUFFERIMPL_H

#include<QIUnknownImpl.h>
#include<QIBuffer.h>
#include<QtComPtr.h>

#include"qcriticalsection.h"


#define DEFAULT_BUFFER_SIZE			512
#define DEFAULT_BLOCK_SIZE			1024

template<typename LockType = QNullCriticalSection>
class QBufferImp: public QIBuffer,private QIUnknownImpl, public QByteArray
{
public:
    typedef QStackLockWrapper<LockType> SRVATUOLOCK;
    QTCOM_ADDREF_RELEASE
    QTCOM_QUERYINTERFACE_BEGIN(QIBuffer)
        QTCOM_QUERYINTERFACE_ENTRY(QIBuffer)
    QTCOM_QUERYINTERFACE_END

    QHRESULT init_class(QIUnknown* ,QIUnknown*)
    {
        return QS_OK;
    }


    QBufferImp()
    {
    }

    virtual ~QBufferImp(void)
    {  
    }
    virtual QHRESULT SetBuffer(const char* lpBuf,ulong dwSize,ulong dwStartIndex = 0)
    {
        SRVATUOLOCK lock(m_lock);

        ulong m_dwDataSize = this->size();

        QRASSERT(lpBuf != NULL,QE_INVALIDARG);
        QRASSERT(dwSize>0,QS_OK);
        QRASSERT(dwStartIndex <= m_dwDataSize,QE_INVALIDARG);


        this->insert(dwStartIndex, lpBuf, dwSize);

        return QS_OK;
    }

    virtual QHRESULT setBuffer(const QByteArray& data)
    {
        QByteArray* array = static_cast<QByteArray*>(this);
        *array = data;
        return QS_OK;
    }

    virtual QHRESULT GetBuffer(const char* lpoutData, ulong dwSize,ulong* pDwReadCount,ulong dwStartIndex = 0)
    {
        SRVATUOLOCK lock(m_lock);

        ulong m_dwDataSize = this->size();
        QRASSERT(lpoutData != nullptr &&  pDwReadCount != nullptr,QE_INVALIDARG);
        QRASSERT(dwStartIndex <= m_dwDataSize,QE_INVALIDARG);
        ulong dwTempSize = std::min(dwSize,m_dwDataSize-dwStartIndex);

        (void)memcmp(lpoutData,this->data() + dwStartIndex , dwTempSize);
        *pDwReadCount = dwTempSize;


        return QS_OK;
    }

    virtual bool isEmpty()
    {
        return QByteArray::isEmpty();
    }

    virtual int size()
    {
        SRVATUOLOCK lock(m_lock);
        return QByteArray::size();
    }

    virtual QHRESULT Clone(QIBuffer** lpIBuufer)
    {
        SRVATUOLOCK lock(m_lock);
        QRASSERT(lpIBuufer != NULL,QE_INVALIDARG);

        *lpIBuufer = NULL;
        QBufferImp* p = new QBufferImp();
        QRASSERT(p, QE_UNEXPECTED);
        p->append(*this);

        return p->QueryInterface(qt_uuidof(QIBuffer), (void**)lpIBuufer);
    }

    virtual void Append(const QByteArray& data)
    {
        SRVATUOLOCK lock(m_lock);
        this->append(data);
    }

    virtual void Append(const QString& data)
    {
        this->append(data);
    }

    virtual void Append(const char* data)
    {
        this->append(data);
    }

    virtual void Append(const char *str, int len)
    {
        this->append(str, len);
    }

    virtual void Append(QIBuffer* buffer)
    {
        if( buffer )
            this->append(buffer->getByteArray());
    }

    virtual QHRESULT AddTail(const char* lpBuf,ulong dwSize)
    {
        this->append(lpBuf, dwSize);
        return QS_OK;
    }

    virtual QHRESULT AddHead(const char* lpBuf,ulong dwSize)
    {
        SRVATUOLOCK lock(m_lock);

        this->insert(0,lpBuf, dwSize);

        return QS_OK;
    }

    virtual const char* data()
    {
        return QByteArray::data();
    }

    virtual void SetDataSize(ulong dwDataSize)
    {
        SRVATUOLOCK lock(m_lock);
        this->reserve(dwDataSize);
    }

    virtual void clear()
    {
        SRVATUOLOCK lock(m_lock);
        QByteArray::clear();
    }

    virtual QByteArray& getByteArray()
    {
        return *this;
    }

private:
    LockType m_lock;
};

#endif // QBUFFERIMPL_H
