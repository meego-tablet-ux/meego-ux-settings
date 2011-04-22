/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SHAREDCREDENTIALS_HPP
#define MEEGO_SHAREDCREDENTIALS_HPP

#include "SharedCredentialsProcessor.hpp"

#include <SignOn/AuthService>


namespace SignOn
{
  class AuthService;
}


/**
 * @class SharedCredentials
 *
 * @brief Process credentials using the given processing strategy.
 */
class SharedCredentials : public QObject
{

  Q_OBJECT

public:

  SharedCredentials(QObject * parent = 0);

  /**
   * Process credentials through the provided credentials processing
   * strategy.
   *
   * Success is reported asynchronously through the
   * strategy-specific signal (e.g. stored(), credentials(),
   * removed()), failure through the strategy's "error"
   * signal.
   */
  void processCredentials(SharedCredentialsProcessor * processor,
			  QString provider);

private slots:

  void identities(const QList<SignOn::IdentityInfo> &identityList);

private:

  Q_DISABLE_COPY(SharedCredentials)

  /// Identity info for provider found.
  void identityFound(const SignOn::IdentityInfo &info);

  /// Identity for provider does not yet exist.
  void identityNotFound();

private:

  /// SSO provider (e.g. google.com).
  QString m_provider;

  /// Credentials processing strategy (e.g. store, retrieve, remove).
  SharedCredentialsProcessor * m_processor;

  /// Underlying SSO authentication service.
  SignOn::AuthService * m_service;

};

#endif  /* MEEGO_SHAREDCREDENTIALS_HPP */
