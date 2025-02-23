QT       += core gui
QT       += svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editorwidget.cpp \
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
    flowlayout.cpp

HEADERS += \
    editorwidget.h \
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
    flowlayout.h

FORMS += \
    editorwidget.ui \
    mainwindow.ui \
    snippetpreviewbox.ui \
    snippetsettingspopup.ui \
    tagviewer.ui

RESOURCES += \
    images/ \
    fonts/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
