#-------------------------------------------------
#
# Project created by QtCreator 2011-05-14T01:18:19
#
#-------------------------------------------------

QT       += core script
TARGET = plotter
TEMPLATE = lib
CONFIG += plugin

DEFINES += PLOTTER_LIBRARY

SOURCES += plotter.cpp

HEADERS += plotter.h\
    plotterinterface.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE2C568AC
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = plotter.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
