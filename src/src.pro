TEMPLATE = app
DESTDIR = ../bin
TARGET = GameTest

QT += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += lrelease
CONFIG += embed_translations

SOURCES += \
    database.cpp \
    inventory.cpp \
    inventorymodel.cpp \
    inventoryview.cpp \
    item.cpp \
    itemdelegate.cpp \
    itemmimedata.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    database.h \
    inventory.h \
    inventorymodel.h \
    inventoryview.h \
    item.h \
    itemdelegate.h \
    itemmimedata.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    GameTest_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
