QT = core xml

TEMPLATE = lib
DEFINES += COMENTRY_LIBRARY

DESTDIR = ../../bin

CONFIG += c++11
INCLUDEPATH+= ../../

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comentry.cpp \
    qclasscontainer.cpp \
    qplugincontainer.cpp \
    qtcomapplication.cpp

HEADERS += \
    comentry_global.h \
    comentry.h \
    qclasscontainer.h \
    qplugincontainer.h \
    qrunningobjecttable.h \
    qtcomapplication.h



# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
