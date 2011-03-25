/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SYNC_CALENDAR_MODEL_HPP
#define MEEGO_SYNC_CALENDAR_MODEL_HPP

#include "StorageModel.hpp"


namespace MeeGo
{
  namespace Sync
  {

    /**
     * List model used during population of the calendar sync profile
     * list presented in ther sync UI/applet.
     */
    class CalendarModel : public StorageModel
    {
      Q_OBJECT

    public:

      CalendarModel(QObject * parent = 0);
      virtual ~CalendarModel();

    };

  }
}


#endif /* MEEGO_SYNC_CALENDAR_MODEL_HPP */
