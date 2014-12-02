#-------------------------------------------------
#
# Project created by QtCreator 2014-11-02T13:06:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app

SOURCES += main.cpp \
    MainScene.cpp \
    GameAreaScene.cpp

HEADERS  += \
    MainScene.h \
    Preview.h \
    GameAreaScene.h

RESOURCES += \
    resources.qrc
