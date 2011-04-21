TEMPLATE = lib
QT += declarative \
	dbus \
	xml \
	xmlpatterns
CONFIG += qt \
    plugin \
    dbus \
    link_pkgconfig \
    ofono-qt
PKGCONFIG += bluetooth-qt \
	connman-qt4 \
	mlite \
	timed \
	gnome-keyring-1 \
	gconf-2.0 \
	ofono-qt \
	MeegoImFramework

system(qdbusxml2cpp -c ConnectionContext -p connectioncontext -N connectioncontext.xml)

TARGET = $$qtLibraryTarget($$TARGET)
OBJECTS_DIR = .obj
MOC_DIR = .moc

# Input
SOURCES += components.cpp \
	declarativesettingsmodel.cpp \
    timesettings.cpp \
    BrowserSettingModel.cpp \
    localesettings.cpp \
    cellularsettings.cpp \
    connectioncontext.cpp \
    notificationtypes.cpp \
    notificationmaker.cpp

OTHER_FILES += qmldir \ 
    connectioncontext.xml
HEADERS += components.h \ 
    declarativesettingsmodel.h \
    timesettings.h \
    BrowserSettingModel.h \
    localesettings.h \
    cellularsettings.h \
    connectioncontext.h \
    notificationtypes.h \
    notificationmaker.h

qmldir.files += qmldir *.qml *.xml
qmldir.path = $$[QT_INSTALL_IMPORTS]/MeeGo/Settings

target.path = $$[QT_INSTALL_IMPORTS]/MeeGo/Settings

INSTALLS += qmldir target
