/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_STORE_CREDENTIALS_PROCESSOR_HPP
#define MEEGO_STORE_CREDENTIALS_PROCESSOR_HPP


#include "SharedCredentialsProcessor.hpp"


/**
 * @class StoreCredentialsProcessor
 *
 * @brief Credentials storage handling strategy implementation.
 *
 * This shared credentials strategy creates or updates credentials,
 * and stores them in SSO.
 */
class StoreCredentialsProcessor : public SharedCredentialsProcessor
{

  Q_OBJECT

public:

  StoreCredentialsProcessor(QString const & username,
			    QString const & password,
			    QObject * parent = 0);
  virtual ~StoreCredentialsProcessor();
  virtual void newIdentity(QString const & provider);

signals:

  /// Signal triggered when shared credentials have been stored.
  void stored(quint32 id);

private slots:

  /// Slot called when shared credentials have been stored.
  void credentialsStored(const quint32 id);

private:

  Q_DISABLE_COPY(StoreCredentialsProcessor)

  virtual void processIdentity_i(const SignOn::IdentityInfo & info);

private:

  QString const & m_username;
  QString const & m_password;

};


#endif  /* MEEGO_STORE_CREDENTIALS_PROCESSOR_HPP */
