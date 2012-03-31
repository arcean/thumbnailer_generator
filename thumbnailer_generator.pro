#-------------------------------------------------
#
# Project created by QtCreator 2012-03-31T14:15:40
#
#-------------------------------------------------

QT       += core

TARGET = thumbnailer_generator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lthumbnailer


SOURCES += main.cpp

contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/thumbnailer_generator/bin
    INSTALLS += target
}

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

HEADERS += \
    header.h
