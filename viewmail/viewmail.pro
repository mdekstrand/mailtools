#-------------------------------------------------
#
# Project created by QtCreator 2014-06-07T20:19:05
#
#-------------------------------------------------

QT       += core gui widgets network webkit

CONFIG += link_pkgconfig

TARGET = viewmail
TEMPLATE = app

MOC_DIR = build
OBJECTS_DIR = build
DESTDIR = build

SOURCES += main.cpp\
        mailview.cpp \
    htmlmail.cpp \
    mailnetworkmanager.cpp \
    statichttpreply.cpp

HEADERS  += mailview.h \
    htmlmail.h \
    mailnetworkmanager.h \
    statichttpreply.h

PKGCONFIG += vmime
QMAKE_CXXFLAGS += -std=gnu++11
