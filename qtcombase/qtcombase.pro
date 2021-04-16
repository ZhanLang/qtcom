QT -= gui

TEMPLATE = lib
DEFINES += QTCOMBASE_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH+= ../include
DESTDIR = "../bin"
SOURCES += \
    qtcombase.cpp

HEADERS += \
    qbufferimpl.h \
    qibuffer.h \
    qtcombase_global.h \
    qtcombase.h

TRANSLATIONS += \
    qtcombase_zh_CN.ts

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target