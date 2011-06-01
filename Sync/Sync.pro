TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

CONFIG += link_pkgconfig \
    qt \
    plugin \
    debug \
    warn_on

PKGCONFIG += syncfwclient syncprofile mlite syncevolution-qt-dbus

QT += declarative xml

HEADERS = \
    AllVisibleProfilesModel.hpp \
    CalendarModel.hpp \
    ContactsModel.hpp \
    FrameworkClient.hpp \
    QProperty.h \
    ServiceInfo.hpp \
    StorageModel.hpp \
    SyncEvoFrameworkClient.h \
    SyncEvoStatic.h \
    SyncEvoStorageModel.h \
    SyncEvoStorageModelItem.h \
    UiPlugin.hpp

SOURCES = \
    AllVisibleProfilesModel.cpp \
    CalendarModel.cpp \
    ContactsModel.cpp \
    FrameworkClient.cpp \
    QProperty.cpp \
    ServiceInfo.cpp \
    StorageModel.cpp \
    SyncEvoFrameworkClient.cpp \
    SyncEvoStatic.cpp \
    SyncEvoStorageModel.cpp \
    SyncEvoStorageModelItem.cpp \
    UiPlugin.cpp

OTHER_FILES += qmldir

qmldir.files += qmldir
qmldir.path = $$[QT_INSTALL_IMPORTS]/MeeGo/Sync

TARGET = $$qtLibraryTarget(Sync)
target.path = $$[QT_INSTALL_IMPORTS]/MeeGo/Sync

INSTALLS += target qmldir
