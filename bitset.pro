# BitSet Qt Project
# Вариант 7: Практика - Битовое множество
# Qt 4.11 compatible

TEMPLATE = app
TARGET = bitset
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += qt warn_on

QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp \
           mainwindow.cpp \
           bitset.cpp

HEADERS += mainwindow.h \
           bitset.h

# Compiler flags for C++11/C++14 compatibility
QMAKE_CXXFLAGS += -std=c++11
