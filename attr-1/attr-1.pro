TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH += /usr/local/include/igraph
SOURCES += main.c \
    chinese.c

LIBS += -ligraph

DISTFILES += \
    unnamed0.gml \
    celegansneural.gml \
    karate.gml

HEADERS += \
    chinese.h \
    mario.h
