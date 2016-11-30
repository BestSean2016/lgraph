TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/local/include/igraph

SOURCES += main.cpp

LIBS += -ligraph

DISTFILES +=

HEADERS += \
    pipegen.h
