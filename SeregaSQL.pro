QT += core
QT -= gui

CONFIG += c++11
QMAKE_CXXFLAGS+=-std=c++11
TARGET = SeregaSQL
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    create_table.cpp \
    errors.cpp

HEADERS += \
    my_column.h