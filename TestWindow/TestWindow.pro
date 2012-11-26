#-------------------------------------------------
#
# Project created by QtCreator 2011-03-03T02:30:47
#
#-------------------------------------------------

QT       += core gui webkit script scripttools

TARGET = TestWindow
TEMPLATE = app


SOURCES += main.cpp\
    helpdialog.cpp \
    imagedialog.cpp \
    testwindow.cpp \
    qwebviewscriptable.cpp \
    buttonstate.cpp

HEADERS  += \
    helpdialog.h \
    imagedialog.h \
    testwindow.h \
    qwebviewscriptable.h \
    buttonstate.h

FORMS    += help.ui image.ui \
    testwindow.ui

OTHER_FILES += \
    manual.txt

RESOURCES += \
    resources.qrc

LIBS += -L"C:\QT4DEV\psytest\TestWindow\quazip-build-desktop-Qt_4_8_1_for_Desktop_-_MinGW__Qt_SDK_________\quazip\debug" -lquazip1

INCLUDEPATH += quazip-0.5/quazip/ quazip-0.5/quazip/zlib-1.2.7















