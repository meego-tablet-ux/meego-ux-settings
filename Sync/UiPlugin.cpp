/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "UiPlugin.hpp"
#include "FrameworkClient.hpp"
#include "CalendarModel.hpp"
#include "ContactsModel.hpp"
#include "AllVisibleProfilesModel.hpp"

#include <qdeclarative.h>


void
MeeGo::Sync::UiPlugin::registerTypes(char const * uri)
{
  static int const MAJOR_VERSION = 0;
  static int const MINOR_VERSION = 1;

  // Register the class the exposes the sync operations to the QML
  // with the metatype system.
  qmlRegisterType<FrameworkClient>(uri,
				   MAJOR_VERSION,
				   MINOR_VERSION,
				   "SyncFwClient");

  // ... and similarly for our sync service list models.
  qmlRegisterType<CalendarModel>(uri,
				 MAJOR_VERSION,
				 MINOR_VERSION,
				 "SyncCalendarModel");

  qmlRegisterType<ContactsModel>(uri,
				 MAJOR_VERSION,
				 MINOR_VERSION,
				 "SyncContactsModel");

  qmlRegisterType<AllVisibleProfilesModel>(uri,
					   MAJOR_VERSION,
					   MINOR_VERSION,
					   "AllVisibleSyncProfilesModel");
}


Q_EXPORT_PLUGIN2(syncuiplugin, MeeGo::Sync::UiPlugin);
