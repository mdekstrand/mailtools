#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T20:19:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += link_pkgconfig

TARGET = mailtools
TEMPLATE = app


SOURCES += main.cpp\
        mailview.cpp \
    htmlmail.cpp

HEADERS  += mailview.h \
    htmlmail.h

PKGCONFIG += vmime
QMAKE_CXXFLAGS += -std=gnu++11
