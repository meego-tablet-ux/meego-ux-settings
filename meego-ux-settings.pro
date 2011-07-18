VERSION = 0.4.4
TEMPLATE = subdirs

CONFIG += ordered

sync_settings {
    DEFINES += SYNC_SETTINGS
}

sync_settings: TRANS_SOURCES = \
    Sync/SyncEvoFrameworkClient.cpp \
    Sync/SyncEvoStatic.cpp

OTHER_FILES += \
    *.qml \
    Bluetooth/*.js \
    Bluetooth/*.qml \
    Browser/*.qml \
    Personalize/*.qml \
    Notifications/*.qml \
    Settings/*.qml \
    Sync/*.qml \
    General/*.qml \
    Wifi/*.js \
    Wifi/*.qml \
    Panels/*.qml

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

qmlfiles.files += *.qml Wifi General Browser Personalize Notifications Panels
qmlfiles.path = $$INSTALL_ROOT/usr/share/$$TARGET

bluetooth.files = Bluetooth/*.qml Bluetooth/helper.js
bluetooth.path = $$INSTALL_ROOT/usr/share/$$TARGET/Bluetooth

sync.files = Sync/*.qml
sync.path = $$INSTALL_ROOT/usr/share/$$TARGET/Sync

builtinsettings.files += apps/*.desktop
builtinsettings.path = $$INSTALL_ROOT/usr/share/$$TARGET/apps/

desktop.files = meego-ux-settings.desktop
desktop.path = $$INSTALL_ROOT/usr/share/applications/

INSTALLS += qmlfiles builtinsettings bluetooth desktop
sync_settings:INSTALLS += sync
