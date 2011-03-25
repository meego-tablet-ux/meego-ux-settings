/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SYNC_CONTACTS_MODEL_HPP
#define MEEGO_SYNC_CONTACTS_MODEL_HPP

#include "StorageModel.hpp"


namespace MeeGo
{
  namespace Sync
  {

    /**
     * List model used during population of the contacts sync profile
     * list presented in ther sync UI/applet.
     */
    class ContactsModel : public StorageModel
    {
      Q_OBJECT

    public:

      ContactsModel(QObject * parent = 0);
      virtual ~ContactsModel();

    };

  }
}


#endif /* MEEGO_SYNC_CONTACTS_MODEL_HPP */
