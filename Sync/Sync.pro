TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

CONFIG += link_pkgconfig \
    qt \
    plugin \
    debug \
    warn_on

PKGCONFIG += mlite syncevolution-qt-dbus

QT += declarative xml

HEADERS = \
    QProperty.h \
    SyncEvoFrameworkClient.h \
    SyncEvoStatic.h \
    SyncEvoStorageModel.h \
    SyncEvoStorageModelItem.h \
    UiPlugin.hpp

SOURCES = \
    QProperty.cpp \
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
