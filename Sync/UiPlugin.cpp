/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "UiPlugin.hpp"
#include "SyncEvoStorageModel.h"
#include "SyncEvoFrameworkClient.h"

#include <qdeclarative.h>

void
MeeGo::Sync::UiPlugin::registerTypes(char const * uri)
{
  static int const MAJOR_VERSION = 0;
  static int const MINOR_VERSION = 1;

  // Register the class the exposes the sync operations to the QML
  // with the metatype system.
  qmlRegisterType<SyncEvoFrameworkClient>(uri,
				   MAJOR_VERSION,
				   MINOR_VERSION,
				   "SyncFwClient");

  qmlRegisterType<SyncEvoStorageModel>(uri,
					   MAJOR_VERSION,
					   MINOR_VERSION,
					   "AllVisibleSyncProfilesModel");
}

Q_EXPORT_PLUGIN2(syncuiplugin, MeeGo::Sync::UiPlugin);
