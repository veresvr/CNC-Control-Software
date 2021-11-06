#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T22:59:57
#
#-------------------------------------------------

QT       += core gui
QT      += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = my_shag
TEMPLATE = app


SOURCES += main.cpp\
    filesparsing.cpp \
        mainwindow.cpp \
    port.cpp \
    settingscom.cpp \
    firmware.cpp

HEADERS  += mainwindow.h \
    filesparsing.h \
    port.h \
    settingscom.h \
    firmware.h

FORMS    += mainwindow.ui \
    settingscom.ui \
    firmware.ui

RESOURCES += \
    images.qrc
