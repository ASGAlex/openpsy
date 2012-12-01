#-------------------------------------------------
#
# Project created by QtCreator 2011-03-03T02:30:47
#
#-------------------------------------------------

QT       += core gui webkit script scripttools

TARGET = TestWindow

TEMPLATE = app


SOURCES += \
    main.cpp\
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

FORMS += \
    help.ui \
    image.ui \
    testwindow.ui

OTHER_FILES += \
    manual.txt

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/quazip/quazip/release/ -lquazip1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/quazip/quazip/debug/ -lquazip1 
else:unix: LIBS += -L$$OUT_PWD/quazip/quazip/ -lquazip

INCLUDEPATH += $$PWD/quazip $$PWD/quazip/quazip/zlib-1.2.7
DEPENDPATH += $$PWD/quazip/quazip
