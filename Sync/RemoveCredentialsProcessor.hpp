/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_REMOVE_CREDENTIALS_PROCESSOR_HPP
#define MEEGO_REMOVE_CREDENTIALS_PROCESSOR_HPP


#include "SharedCredentialsProcessor.hpp"


/**
 * @class StoreSharedCredentials
 *
 * @brief Credentials storage handling strategy implementation.
 *
 * This shared credentials strategy creates or updates credentials,
 * and stores them in SSO.
 */
class RemoveCredentialsProcessor : public SharedCredentialsProcessor
{

  Q_OBJECT

public:

  RemoveCredentialsProcessor(QObject * parent = 0);
  virtual ~RemoveCredentialsProcessor();
  virtual void newIdentity(QString const & provider);

signals:

  /// Signal triggered when shared credentials have been removed.
  void removed();

private slots:

  /// Slot called when shared credentials have been removed.
  void credentialsRemoved();

private:

  virtual void processIdentity_i(const SignOn::IdentityInfo & info);

};


#endif  /* MEEGO_REMOVE_CREDENTIALS_PROCESSOR_HPP */
