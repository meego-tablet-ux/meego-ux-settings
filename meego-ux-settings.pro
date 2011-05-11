VERSION = 0.2.8
TEMPLATE = subdirs 

CONFIG += ordered

TRANS_SOURCES = \
    Sync/AllVisibleProfilesModel.cpp \
    Sync/FrameworkClient.cpp \
    Sync/UiPlugin.cpp \
    Sync/CalendarModel.cpp \
    Sync/SharedCredentials.cpp \
    Sync/ContactsModel.cpp \
    Sync/StorageModel.cpp \
    Sync/AllVisibleProfilesModel.hpp \
    Sync/FrameworkClient.hpp \
    Sync/UiPlugin.hpp \
    Sync/CalendarModel.hpp \
    Sync/SharedCredentials.hpp \
    Sync/ContactsModel.hpp \
    Sync/StorageModel.hpp

OTHER_FILES += \
    main.qml \
    Bluetooth/*.js \
    Bluetooth/*.qml \
    Browser/*.qml \
    Language/*.qml \
    Panels/*.qml \
    Personalize/*.qml \
    Notifications/*.qml \
    Settings/*.qml \
    Sync/*.qml \
    TimeDate/*.qml \
    Volume/*.qml \
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


SUBDIRS += Settings Sync Example Hacks

qmlfiles.files += *.qml Wifi Volume Panels TimeDate Browser Language Personalize Notifications
qmlfiles.path = $$INSTALL_ROOT/usr/share/$$TARGET

bluetooth.files = Bluetooth/*.qml Bluetooth/helper.js
bluetooth.path = $$INSTALL_ROOT/usr/share/$$TARGET/Bluetooth

sync.files = Sync/*.qml
sync.path = $$INSTALL_ROOT/usr/share/$$TARGET/Sync

builtinsettings.files += apps/*.desktop
builtinsettings.path = $$INSTALL_ROOT/usr/share/$$TARGET/apps/

INSTALLS += qmlfiles builtinsettings bluetooth sync
