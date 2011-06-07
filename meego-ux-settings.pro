VERSION = 0.3.6
TEMPLATE = subdirs

CONFIG += ordered sync_settings

sync_settings {
    DEFINES += SYNC_SETTINGS
}

sync_settings: TRANS_SOURCES = \
    Sync/AllVisibleProfilesModel.cpp \
    Sync/FrameworkClient.cpp \
    Sync/UiPlugin.cpp \
    Sync/CalendarModel.cpp \
    Sync/ContactsModel.cpp \
    Sync/StorageModel.cpp \
    Sync/AllVisibleProfilesModel.hpp \
    Sync/FrameworkClient.hpp \
    Sync/UiPlugin.hpp \
    Sync/CalendarModel.hpp \
    Sync/ContactsModel.hpp \
    Sync/StorageModel.hpp

OTHER_FILES += \
    main.qml \
    Bluetooth/*.js \
    Bluetooth/*.qml \
    Browser/*.qml \
    Language/*.qml \
    Personalize/*.qml \
    Notifications/*.qml \
    Settings/*.qml \
    Sync/*.qml \
    General/*.qml \
    Wifi/*.js \
    Wifi/*.qml

TRANSLATIONS += $${TRANS_SOURCES} $${OTHER_FILES} $${HEADERS}
PROJECT_NAME = meego-ux-settings

dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION} &&
dist.commands += git clone . $${PROJECT_NAME}-$${VERSION} &&
dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION}/.git &&
dist.commands += mkdir -p $${PROJECT_NAME}-$${VERSION}/ts &&
dist.commands += lupdate $${TRANSLATIONS} -ts $${PROJECT_NAME}-$${VERSION}/ts/$${PROJECT_NAME}.ts &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION}
QMAKE_EXTRA_TARGETS += dist


SUBDIRS += Settings Example Hacks
sync_settings:SUBDIRS += Sync

qmlfiles.files += *.qml Wifi General Browser Language Personalize Notifications
qmlfiles.path = $$INSTALL_ROOT/usr/share/$$TARGET

bluetooth.files = Bluetooth/*.qml Bluetooth/helper.js
bluetooth.path = $$INSTALL_ROOT/usr/share/$$TARGET/Bluetooth

sync.files = Sync/*.qml
sync.path = $$INSTALL_ROOT/usr/share/$$TARGET/Sync

builtinsettings.files += apps/*.desktop
builtinsettings.path = $$INSTALL_ROOT/usr/share/$$TARGET/apps/

INSTALLS += qmlfiles builtinsettings bluetooth
sync_settings:INSTALLS += sync
