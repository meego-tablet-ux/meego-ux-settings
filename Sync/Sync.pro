TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

CONFIG += link_pkgconfig \
    qt \
    plugin \
    debug \
    warn_on

PKGCONFIG += syncfwclient syncprofile libsignon-qt mlite

QT += declarative xml

HEADERS = \
    AllVisibleProfilesModel.hpp \
    CalendarModel.hpp \
    ContactsModel.hpp \
    FrameworkClient.hpp \
    RemoveCredentialsProcessor.hpp \
    RetrieveCredentialsProcessor.hpp \
    ServiceInfo.hpp \
    SharedCredentials.hpp \
    SharedCredentialsProcessor.hpp \
    StorageModel.hpp \
    StoreCredentialsProcessor.hpp \
    UiPlugin.hpp

SOURCES = \
    AllVisibleProfilesModel.cpp \
    CalendarModel.cpp \
    ContactsModel.cpp \
    FrameworkClient.cpp \
    RemoveCredentialsProcessor.cpp \
    RetrieveCredentialsProcessor.cpp \
    ServiceInfo.cpp \
    SharedCredentials.cpp \
    SharedCredentialsProcessor.cpp \
    StorageModel.cpp \
    StoreCredentialsProcessor.cpp \
    UiPlugin.cpp

OTHER_FILES += qmldir

qmldir.files += qmldir
qmldir.path = $$[QT_INSTALL_IMPORTS]/MeeGo/Sync

TARGET = $$qtLibraryTarget(Sync)
target.path = $$[QT_INSTALL_IMPORTS]/MeeGo/Sync

INSTALLS += target qmldir
