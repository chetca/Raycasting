QT += core gui widgets

TARGET = Raycasting
TEMPLATE = app

SOURCES = raycasting.cpp \
    main.cpp \
    readlevel.cpp

RESOURCES += raycasting.qrc

HEADERS += \
    readlevel.h

DISTFILES += \
    level.map
