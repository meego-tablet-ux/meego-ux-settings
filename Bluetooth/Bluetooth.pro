TEMPLATE = app
QT += declarative dbus opengl

TARGET = bluetoothagent

CONFIG += qt \
    dbus \
    link_pkgconfig \
    meegoqmllauncher


PKGCONFIG += bluetooth-qt meegoqmllauncher

SOURCES += main.cpp \
    bluetoothagent.cpp

OTHER_FILES += *.qml


HEADERS += \
    bluetoothagent.h
