QT += core gui widgets

CONFIG += c++11

TARGET = Raycasting
TEMPLATE = app

SOURCES = raycasting.cpp \
    main.cpp \
    readlevel.cpp

RESOURCES += raycasting.qrc

HEADERS += \
    readlevel.h \
    raycasting.h

DISTFILES += \
    level.map
