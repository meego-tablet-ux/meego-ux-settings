TEMPLATE = subdirs

OTHER_FILES += declarativeexample.xml declarativeexample.desktop

desktop.files = *.desktop
desktop.path = $$INSTALL_ROOT/usr/share/meego-ux-settings/apps

xml.files = *.xml
xml.path = $$INSTALL_ROOT/usr/share/meego-ux-settings/Example

INSTALLS += desktop xml
