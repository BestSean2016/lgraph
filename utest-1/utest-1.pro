TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../include


DEFINES += __USING_MYSQL__

SOURCES += test-1.cpp \
    ../src/chinese.c \
    ../src/mario_mysql.c

###################### unix ############################
unix {
    DEFINES += _UNIX

    message("Building for unix")
    INCLUDEPATH += /usr/local/include

    LIBS += -lpthread -lrt
    LIBS += -L/usr/local/lib -lgtest -lgtest_main

    target.path = /usr/local/bin/lgraph
    INSTALLS += target

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = utest-1d
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = utest-1
        message("Build for release version")
    }
}

###################### windows #########################
windows {
    DEFINES += _WINDOWS

    message("Building for Windows")

    INCLUDEPATH += D:/projects/md/gtest

    CONFIG(debug, debug|release) {
        DEFINES += _DEBUG_
        TARGET = utest-1d
        LIBS += -LD:/projects/md/gtest/win64 -lgtest-d -lgtest_main-d
        message("Build for Debug version")
    }
    CONFIG(release, debug|release) {
        TARGET = utest-1
        LIBS += -LD:/projects/md/gtest/win64 -lgtest -lgtest_main
        message("Build for release version")
    }

}
