QT       += core gui
QT       += svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    browselangwidget.cpp \
    browsetagwidget.cpp \
    editorwidget.cpp \
    filterwidget.cpp \
    langholder.cpp \
    main.cpp \
    mainwindow.cpp \
    searchsyetem.cpp \
    snippetbaseclass.cpp \
    snippetc.cpp \
    snippetcpp.cpp \
    snippetcss.cpp \
    snippetcustom.cpp \
    snippetjava.cpp \
    snippetliveclass.cpp \
    snippetpreviewbox.cpp \
    snippetpy.cpp \
    snippetsettingspopup.cpp \
    tagholder.cpp \
    tagviewer.cpp \
    flowlayout.cpp \
    cryptographicagent.cpp

HEADERS += \
    browselangwidget.h \
    browsetagwidget.h \
    editorwidget.h \
    filterwidget.h \
    langholder.h \
    mainwindow.h \
    predefines.h \
    searchsyetem.h \
    snippetbaseclass.h \
    snippetc.h \
    snippetcpp.h \
    snippetcss.h \
    snippetcustom.h \
    snippetjava.h \
    snippetliveclass.h \
    snippetpreviewbox.h \
    helperFunctions.h \
    snippetpy.h \
    snippetsettingspopup.h \
    tagholder.h \
    tagviewer.h \
    flowlayout.h \
    cryptographicagent.h

FORMS += \
    browselangwidget.ui \
    browsetagwidget.ui \
    editorwidget.ui \
    filterwidget.ui \
    mainwindow.ui \
    snippetpreviewbox.ui \
    snippetsettingspopup.ui \
    tagviewer.ui \
    cryptographicagent.ui

RESOURCES += \
    images/ \
    fonts/


#-------------------------------------------------
# OpenSSL configuration - Platform-Specific
#-------------------------------------------------
win32 {
    INCLUDEPATH += $$PWD/openssl_libs/windows/include
    LIBS += -L$$PWD/openssl_libs/windows/lib64 -lssl -lcrypto -lws2_32 -lcrypt32
}

macx {
    INCLUDEPATH += $$PWD/openssl_libs/mac/include
    LIBS += -L$$PWD/openssl_libs/mac/lib -lssl -lcrypto
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 15.0
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
