CONFIG += c++11
TARGET = SeregaSQL
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    errors.cpp \
    functions.cpp \
    errors_c.cpp

HEADERS += \
    my_column.h
