TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    EvaluateEngine.cpp \
    MoveGenerator.cpp \
    SearchEngine.cpp

HEADERS += \
    EvaluateEngine.h \
    utils.h \
    MoveGenerator.h \
    SearchEngine.h
