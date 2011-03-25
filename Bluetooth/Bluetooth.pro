TEMPLATE = app
QT += declarative dbus

TARGET = bluetoothagent

CONFIG += qt \
    dbus \
    link_pkgconfig

PKGCONFIG += bluetooth-qt

SOURCES += main.cpp \
    bluetoothagent.cpp

HEADERS += \
    bluetoothagent.h
