QT += core gui widgets

CONFIG += c++11

TARGET = Raycasting
TEMPLATE = app

SOURCES = raycasting.cpp \
    main.cpp \
    readlevel.cpp \
    menu.cpp \
    redactor.cpp

RESOURCES += raycasting.qrc

HEADERS += \
    readlevel.h \
    raycasting.h \
    menu.h \
    redactor.h

DISTFILES += \
    level.map

FORMS += \
    menu.ui
