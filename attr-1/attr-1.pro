TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


INCLUDEPATH += /usr/local/include/igraph
SOURCES += main.c

LIBS += -ligraph

DISTFILES += \
    unnamed0.gml \
    celegansneural.gml \
    karate.gml
