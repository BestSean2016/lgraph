TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH += /usr/local/include/igraph
INCLUDEPATH += ../include

SOURCES += main.c \
    ../src/chinese.c

LIBS += -ligraph

DISTFILES += \
    unnamed0.gml \
    celegansneural.gml \
    karate.gml \
    mario.sql

HEADERS += \
    ../include/chinese.h \
    ../include/mario_types.h
