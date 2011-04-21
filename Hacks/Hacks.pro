TEMPLATE=subdirs

OTHER_FILES += *.qml \
    NotificationMaker/*.qml

DESKTOP_FILES = hacks.desktop

desktop.files = hacks.desktop
desktop.path = $$INSTALL_ROOT/usr/share/meego-ux-settings/apps/

qml.files = $$OTHER_FILES
qml.path = $$INSTALL_ROOT/usr/share/meego-ux-settings/Hacks

icon.files = icn_hacks.png
icon.path = $$INSTALL_ROOT/usr/share/pixmaps

INSTALLS += desktop qml icon
