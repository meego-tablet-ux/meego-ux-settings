/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "components.h"

#include <nearbydevicesmodel.h>
#include <bluetoothdevice.h>
#include <bluetoothdevicemodel.h>
#include <networklist.h>
#include <networkitem.h>

#include "localesettings.h"
#include "cellularsettings.h"
#include "timesettings.h"
#include "declarativesettingsmodel.h"
#include "BrowserSettingModel.h"
#include "notificationtypes.h"
#include "notificationmaker.h"

void Components::registerTypes(const char *uri)
{
	qmlRegisterType<NearbyDevicesModel>(uri,0,0,"NearbyDevicesModel");
    qmlRegisterType<BluetoothDevice>(uri,0,0,"BluetoothDevice");
    qmlRegisterType<BluetoothDevicesModel>(uri,0,0,"BluetoothDevicesModel");
	qmlRegisterType<NetworkListModel>(uri,0,1,"NetworkListModel");
	qmlRegisterType<NetworkItemModel>(uri,0,1,"NetworkItemModel");
	qmlRegisterType<BrowserSettingModel>(uri, 0, 1, "BrowserSettingModel");

	qmlRegisterType<TabletSettings::Group>(uri,0,1,"TabletSettingsGroup");
	qmlRegisterType<TabletSettings::Setting>(uri,0,1,"TabletSettingsSetting");
	qmlRegisterType<TabletSettings::Text>(uri,0,1,"TabletSettingsText");
	qmlRegisterType<TabletSettings::Integer>(uri,0,1,"TabletSettingsInteger");
	qmlRegisterType<TabletSettings::Boolean>(uri,0,1,"TabletSettingsBoolean");
	qmlRegisterType<TabletSettings::Selection>(uri,0,1,"TabletSettingsSelection");
	qmlRegisterType<TabletSettings::Option>(uri,0,1,"TabletSettingsSelectionOption");

	qmlRegisterType<DeclarativeSettingsModel>(uri,0,0,"DeclarativeSettingsModel");
	qmlRegisterType<TimeSettings>(uri, 0, 1,"TimeSettings");
	qmlRegisterType<CellularSettings>(uri, 0, 1,"CellularSettingsModel");
	qmlRegisterType<LocaleSettings>(uri, 0, 1,"LocaleSettings");
	qmlRegisterType<NotificationTypes>(uri, 0, 1,"NotificationTypes");
	qmlRegisterType<NotificationMaker>(uri, 0, 1,"NotificationMaker");
}

void Components::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
	Q_UNUSED(engine);
}

Q_EXPORT_PLUGIN(Components);
