TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += __USING_MYSQL__

INCLUDEPATH += /usr/local/include/igraph
INCLUDEPATH += ../include

SOURCES += main.c \
    ../src/chinese.c \
    ../src/mario_mysql.c

LIBS += -ligraph

DISTFILES += \
    unnamed0.gml \
    celegansneural.gml \
    karate.gml \
    mario.sql \
    ../src/mario-host-ip.py \
    config.ini

HEADERS += \
    ../include/chinese.h \
    ../include/mario_types.h \
    ../include/mario_mysql.h
