#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T15:28:49
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = libcore
TEMPLATE = lib

DEFINES += LIBCORE_LIBRARY

SOURCES += core.cpp \
    usersession.cpp \
    eventhandler.cpp \
    ircsession.cpp \
    scrollback.cpp \
    commandprocessor.cpp \
    generic.cpp \
    configuration.cpp \
    exception.cpp \
    factory.cpp \
    autocompletionengine.cpp \
    terminalparser.cpp \
    grumpydsession.cpp \
    networksession.cpp \
    favorite.cpp \
    query.cpp \
    extension.cpp \
    cppextension.cpp \
    hooks.cpp \
    highlighter.cpp \
    sqlite.cpp \
    pythonengine.cpp \
    grumpyscript.cpp

HEADERS += core.h\
        libcore_global.h \
    usersession.h \
    eventhandler.h \
    ircsession.h \
    scrollback.h \
    commandprocessor.h \
    generic.h \
    configuration.h \
    definitions.h \
    exception.h \
    factory.h \
    autocompletionengine.h \
    terminalparser.h \
    grumpydsession.h \
    networksession.h \
    favorite.h \
    query.h \
    extension.h \
    cppextension.h \
    hooks.h \
    highlighter.h \
    sqlite.h \
    pythonengine.h \
    grumpyscript.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../libirc/build-libirc-Desktop-Debug/ -llibirc

INCLUDEPATH += $$PWD/../libirc/build-libirc-Desktop-Debug
DEPENDPATH += $$PWD/../libirc/build-libirc-Desktop-Debug

unix:!macx: LIBS += -L$$PWD/../libirc/build-libircclient-Desktop-Debug/ -llibircclient

INCLUDEPATH += $$PWD/../libirc/build-libircclient-Desktop-Debug
DEPENDPATH += $$PWD/../libirc/build-libircclient-Desktop-Debug
