#ifndef QBUFFERIMPL_H
#define QBUFFERIMPL_H

#include<QIUnknownImpl>
#include<QCriticalSection>
#include<QComPtr>

#include"qibuffer.h"


#define DEFAULT_BUFFER_SIZE			512
#define DEFAULT_BLOCK_SIZE			1024


template<typename T>
struct sysbufmgr
{
    inline T* Alloc(ulong _size)
    {
        if (!_size)
        {
            _size = 1;
        }

        T* p = (T*)malloc(_size *sizeof(T));
        if (p)
        {
            memset(p,0,_size);
        }
        return p;
    }

    inline void  Free(T * _chuck , ulong _size)
    {
        Q_UNUSED(_size)

        if (_chuck)
        {
            free(_chuck );
            _chuck = NULL;
        }
    }

    inline T* ReAlloc(T* _chuck, ulong _size)
    {
        if (_chuck)
        {
            T* p =  (T*)realloc(_chuck, _size * sizeof(T));
            return p;
        }

        return NULL;
    }
};

template<typename LockType = QNullCriticalSection ,typename T2 = sysbufmgr<uchar>>
class QBufferImp: public QIBuffer,private QUnknownImp
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


    QBufferImp(void*)
    {
        m_dwBufSize = 0;
        m_dwDataSize = 0;
        m_lpByteBuf = NULL;
        Reserve();
    }

    virtual ~QBufferImp(void)
    {
        if(m_lpByteBuf)
        {
            m_bufMgr.Free(m_lpByteBuf , m_dwBufSize);
            m_lpByteBuf = NULL;
        }
    }



    QSTDMETHOD(GetRawBuffer)(uchar** lppoutData,ulong* dwBufSize)
    {
        SRVATUOLOCK lock(m_lock);
        QRASSERT(lppoutData != NULL &&  dwBufSize != NULL,QE_INVALIDARG);

        *lppoutData = m_lpByteBuf;
        *dwBufSize = m_dwDataSize;
        return QS_OK;
    }

    QSTDMETHOD(SetBuffer)(uchar* lpBuf,ulong dwSize,ulong dwStartIndex = 0)
    {
        SRVATUOLOCK lock(m_lock);
        QRASSERT(m_lpByteBuf,QE_FAIL);
        QRASSERT(lpBuf != NULL,QE_INVALIDARG);
        QRASSERT(dwSize>0,QS_OK);
        QRASSERT(dwStartIndex <= m_dwDataSize,QE_INVALIDARG);


        if (dwSize + dwStartIndex <=m_dwBufSize)
        {
            memcmp(m_lpByteBuf+dwStartIndex,lpBuf,dwSize);
        }
        else
        {

            ulong dwTempBufLen = dwSize +dwStartIndex + DEFAULT_BLOCK_SIZE;
            m_lpByteBuf = m_bufMgr.ReAlloc(m_lpByteBuf, dwTempBufLen);

            uchar* point = m_lpByteBuf+dwStartIndex;
            memcpy(point, lpBuf, dwSize);

            m_dwBufSize = dwTempBufLen;
        }

        m_dwDataSize = (dwSize+dwStartIndex);

        return QS_OK;
    }

    QSTDMETHOD(GetBuffer)(uchar* lpoutData, ulong dwSize,ulong* pDwReadCount,ulong dwStartIndex = 0)
    {
        SRVATUOLOCK lock(m_lock);
        QRASSERT(m_lpByteBuf,QE_FAIL);
        QRASSERT(lpoutData != nullptr &&  pDwReadCount != nullptr,QE_INVALIDARG);
        QRASSERT(dwStartIndex <= m_dwDataSize,QE_INVALIDARG);
        ulong dwTempSize = std::min(dwSize,m_dwDataSize-dwStartIndex);
        memcmp(lpoutData,m_lpByteBuf+dwStartIndex,dwTempSize);
        *pDwReadCount = dwTempSize;
        return QS_OK;
    }

    QSTDMETHOD(Reserve)(ulong dwDesiredSize = 0)
    {
        dwDesiredSize = dwDesiredSize ? dwDesiredSize : DEFAULT_BUFFER_SIZE;

        SRVATUOLOCK lock(m_lock);
        if (dwDesiredSize <= m_dwBufSize )
        {
            return QS_OK;
        }

        if (m_lpByteBuf == NULL)
        {
            m_lpByteBuf = m_bufMgr.Alloc(dwDesiredSize);
            QRASSERT(m_lpByteBuf,QE_FAIL);
        }
        else
        {
            m_lpByteBuf = m_bufMgr.ReAlloc(m_lpByteBuf, dwDesiredSize);

        }

        m_dwBufSize = dwDesiredSize;

        return QS_OK;
    }

    QSTDMETHOD_(ulong,GetReservedSize)()
    {
        SRVATUOLOCK lock(m_lock);
        return m_dwBufSize;
    }

    QSTDMETHOD_(ulong,GetBufferSize)()
    {
        SRVATUOLOCK lock(m_lock);
        return m_dwDataSize;
    }

    QSTDMETHOD(Clone)(QIBuffer** lpIBuufer)
    {
        SRVATUOLOCK lock(m_lock);
        QRASSERT(lpIBuufer != NULL,QE_INVALIDARG);

        *lpIBuufer = NULL;
        QComPtr<QIBuffer> p(new QBufferImp(nullptr));
        QRASSERT(p, QE_UNEXPECTED);
        p->Reserve(m_dwBufSize);

        uchar* lppoutData = NULL;
        ulong  dwDataSize = 0;
        QRFAILED(GetRawBuffer(&lppoutData,&dwDataSize));
        p->SetBuffer(lppoutData,dwDataSize,0);

        return p->QueryInterface(qt_uuidof(QIBuffer), (void**)lpIBuufer);
    }

    QSTDMETHOD(AddTail)(uchar* lpBuf,ulong dwSize)
    {
        return SetBuffer(lpBuf,dwSize,m_dwDataSize);
    }

    QSTDMETHOD(AddHead)(uchar* lpBuf,ulong dwSize)
    {
        SRVATUOLOCK lock(m_lock);
        QRASSERT(m_lpByteBuf,QE_FAIL);
        QRASSERT(lpBuf != NULL,QE_INVALIDARG);

        if (dwSize + m_dwDataSize <=m_dwBufSize)
        {
            memcmp(m_lpByteBuf+dwSize,m_lpByteBuf,m_dwDataSize);
            memcmp(m_lpByteBuf,lpBuf,dwSize);
        }
        else
        {
            ulong dwTempBufLen = dwSize + m_dwDataSize + DEFAULT_BLOCK_SIZE;
            m_lpByteBuf = m_bufMgr.ReAlloc(m_lpByteBuf, dwTempBufLen);
             memcmp(m_lpByteBuf+dwSize,m_lpByteBuf,m_dwDataSize);
             memcmp(m_lpByteBuf,lpBuf,dwSize);

            m_dwBufSize = dwTempBufLen;

        }

        m_dwDataSize += dwSize;

        return QS_OK;
    }
    QSTDMETHOD_(const uchar*, GetData)()
    {
        return m_lpByteBuf;
    }

    QSTDMETHOD(SetDataSize)(ulong dwDataSize)
    {
        SRVATUOLOCK lock(m_lock);
        QRASSERT(dwDataSize<=m_dwBufSize,QE_FAIL);
        m_dwDataSize = dwDataSize;
        return QS_OK;
    }

    QSTDMETHOD(Clear)()
    {
        m_dwDataSize = 0;
        return QS_OK;
    }

    QSTDMETHOD(ClearAndFree)()
    {
        m_dwDataSize = 0;
        m_bufMgr.Free(m_lpByteBuf , m_dwBufSize);
        m_lpByteBuf = nullptr;

        return Reserve();
    }

private:
    uchar* m_lpByteBuf;
    ulong  m_dwBufSize;
    ulong  m_dwDataSize;
    T2 m_bufMgr;
    LockType m_lock;
};

#endif // QBUFFERIMPL_H
