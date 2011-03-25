/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include "RetrieveCredentialsProcessor.hpp"

#include <SignOn/Identity>
#include <SignOn/SessionData>
#include <SignOn/AuthSession>


RetrieveCredentialsProcessor::RetrieveCredentialsProcessor(QObject * parent)
  : SharedCredentialsProcessor(parent)
  , m_session(0)
{
  qRegisterMetaType<SignOn::SessionData>("SignOn::SessionData");
  qRegisterMetaType<SignOn::AuthSession::AuthSessionError>("AuthSession::AuthSessionError");
}

RetrieveCredentialsProcessor::~RetrieveCredentialsProcessor()
{
  if (!m_identity.isNull() && m_session != 0) {
    m_identity->destroySession(m_session);
    m_session = 0;
  }

  // m_identity is cleaned up in the base class destructor.
}

void RetrieveCredentialsProcessor::newIdentity(QString const & provider)
{
  this->processError(SignOn::Error(SignOn::Error::IdentityNotFound,
				   provider + " not found"));
}

void RetrieveCredentialsProcessor::processIdentity_i(
  SignOn::IdentityInfo const & /* info */)
{
  // Setup an authentication session using the "password" method.
  m_session = m_identity->createSession(QLatin1String("password"));

  connect(m_session, SIGNAL(response(const SignOn::SessionData &)),
	  this, SLOT(response(const SignOn::SessionData &)));

  connect(m_session, SIGNAL(error(const SignOn::Error &)),
	  this, SLOT(processError(const SignOn::Error &)));

  // Get the password!
  m_session->process(SignOn::SessionData(), QLatin1String("password"));
}

bool RetrieveCredentialsProcessor::operationPending() const
{
  return m_session != 0;
}

void RetrieveCredentialsProcessor::response(
  const SignOn::SessionData & sessionData)
{
  // m_identity and m_session should never be null when this slot is
  // called.
  m_identity->destroySession(m_session);
  m_session = 0;

  m_identity.reset();

  emit credentials(sessionData.UserName(), sessionData.Secret());
}
