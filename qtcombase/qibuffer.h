#ifndef QIBUFFER_H
#define QIBUFFER_H

#include<QIUnknown>
struct QIBuffer : public QIUnknown
{
    //获取原始buffer指针
    //这个动作比较危险,之所以要暴露是为了提速
    //dwBufSize 是当前buffer的大小，不是实际内存的大小
    //这个动作时为了提速，少一次copy动作
    //原则上是不要修改lppoutData 里面的东西，如果用这个接口尽量是仅仅读这段内存
    QSTDMETHOD(GetRawBuffer)(uchar** lppoutData,ulong* dwBufSize) = 0;

    //dwStartIndex 是从buffer的那个内存段开始写 , 覆盖原始数据
    QSTDMETHOD(SetBuffer)(uchar* lpBuf,ulong dwSize,ulong dwStartIndex = 0) = 0;

    //将buf中的内存复制到用户指定的内存中
    //lpBuf 和 dwSize 是数据的指针和大小
    //dwStartIndex 是从buffer的那个内存段开始写
    //pDwReadCount 是读取了多少字节
    QSTDMETHOD(GetBuffer)(uchar* lpoutData, ulong dwSize,ulong* pDwReadCount,ulong dwStartIndex = 0) = 0;

    //保证buffer内部的内存是DwDesiredSize 的大小
    //0,默认 256 字节
    QSTDMETHOD(Reserve)(ulong dwDesiredSize = 0) = 0;

    //取得buffer内部的内存是的大小
    QSTDMETHOD_(ulong,GetReservedSize)() = 0;

    //取得当前buffer 中有效数据的大小
    QSTDMETHOD_(ulong,GetBufferSize)() = 0;

    //获取内部数据，不安全
    QSTDMETHOD_(const uchar*, GetData)() = 0;

    //克隆一个跟当前buffer一样的buffer,两个buf互相不干扰
    QSTDMETHOD(Clone)(QIBuffer** lpIBuufer) = 0;

    //将一段buffer增加到尾部，原先数据不变
    //会影响到 数据的大小，内部缓冲的大小
    QSTDMETHOD(AddTail)(uchar* lpBuf,ulong dwSize) = 0;

    //将一段buffer增加到头部，原先数据不变
    //会影响到 数据的大小，内部缓冲的大小
    QSTDMETHOD(AddHead)(uchar* lpBuf,ulong dwSize) = 0;

    //清空内部数据，但是缓冲区长度保持不变
    QSTDMETHOD(Clear)() = 0;

    //清空内部数据，同时释放内存
    QSTDMETHOD(ClearAndFree)() = 0;
};

QT_DEFINE_IID(QIBuffer, "qtcom.i.buffer")

QT_DEFINE_CLSID(CLSID_Buffer, "qtcom.class.buffer")

//safe buffer
QT_DEFINE_CLSID(CLSID_SafeBuffer, "qtcom.class.safebuffer")

#endif // QIBUFFER_H
