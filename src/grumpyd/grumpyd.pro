QT += core network xml
QT -= gui

TARGET = grumpyd
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    corewrapper.cpp \
    grumpyd.cpp \
    listener.cpp \
    session.cpp \
    sleeper.cpp \
    databasebackend.cpp \
    databasexml.cpp \
    user.cpp \
    security.cpp \
    virtualscrollback.cpp \
    scrollbackfactory.cpp

HEADERS += \
    corewrapper.h \
    grumpyd.h \
    listener.h \
    session.h \
    sleeper.h \
    databasebackend.h \
    databasexml.h \
    user.h \
    security.h \
    virtualscrollback.h \
    scrollbackfactory.h

