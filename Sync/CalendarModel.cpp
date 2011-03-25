/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "CalendarModel.hpp"


// @todo Is it possible to automatically determine the calendar
//       storage name?
namespace
{
  char const * STORAGE_NAMES[] = { "hcalendar", 0 };
}

MeeGo::Sync::CalendarModel::CalendarModel(QObject * parent) 
  : StorageModel(STORAGE_NAMES, parent)
{
}

MeeGo::Sync::CalendarModel::~CalendarModel()
{
}
