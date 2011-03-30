/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_RETRIEVE_CREDENTIALS_PROCESSOR_HPP
#define MEEGO_RETRIEVE_CREDENTIALS_PROCESSOR_HPP


#include "SharedCredentialsProcessor.hpp"


namespace SignOn
{
  class AuthSession;
  class SessionData;
}

/**
 * @class RetrieveCredentialsProcessor
 *
 * @brief Credentials retrieval handling strategy implementation.
 *
 * This shared credentials strategy retrieves credentials from SSO.
 */
class RetrieveCredentialsProcessor : public SharedCredentialsProcessor
{

  Q_OBJECT

public:

  RetrieveCredentialsProcessor(QObject * parent = 0);
  virtual ~RetrieveCredentialsProcessor();
  virtual void newIdentity(QString const & provider);

signals:

  /// Signal triggered when shared credentials have been retrieved.
  void credentials(const QString & username, const QString & password);

private slots:

  /// Slot called when shared credentials have been retrieved.
  void response(SignOn::SessionData const & sessionData);

private:

  Q_DISABLE_COPY(RetrieveCredentialsProcessor)

  virtual void processIdentity_i(const SignOn::IdentityInfo & info);
  virtual bool operationPending() const;

private:

  /// Underlying SSO object from which credentials are retrieved.
  SignOn::AuthSession * m_session;

};


#endif  /* MEEGO_RETRIEVE_CREDENTIALS_PROCESSOR_HPP */
