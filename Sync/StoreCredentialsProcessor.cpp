/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include "StoreCredentialsProcessor.hpp"

#include <SignOn/Identity>


StoreCredentialsProcessor::StoreCredentialsProcessor(QString const & username,
						     QString const & password,
						     QObject * parent)
  : SharedCredentialsProcessor(parent)
  , m_username(username)
  , m_password(password)
{
}

StoreCredentialsProcessor::~StoreCredentialsProcessor()
{
}

void StoreCredentialsProcessor::newIdentity(QString const & provider)
{
  // Create new identity from scratch.
  SignOn::IdentityInfo newInfo;
  newInfo.setUserName(m_username);
  newInfo.setSecret(m_password);
  newInfo.setCaption(provider);

  m_identity.reset(SignOn::Identity::newIdentity(newInfo));

  if (m_identity.isNull()) {
    this->processError(SignOn::Error(SignOn::Error::IdentityErr,
				     "identity creation failed"));
  } else {
    connect(m_identity.data(), SIGNAL(credentialsStored(quint32)),
	    this, SLOT(credentialsStored(quint32)));
    connect(m_identity.data(), SIGNAL(error(const SignOn::Error &)),
	    this, SLOT(processError(const SignOn::Error &)));

    m_identity->storeCredentials();
  }
}

void StoreCredentialsProcessor::processIdentity_i(
  SignOn::IdentityInfo const & info)
{
  // Update existing identity.
  SignOn::IdentityInfo newInfo(info);
  newInfo.setUserName(m_username);
  newInfo.setSecret(m_password);

  connect(m_identity.data(), SIGNAL(credentialsStored(quint32)),
	  this, SLOT(credentialsStored(quint32)));
  connect(m_identity.data(), SIGNAL(error(const SignOn::Error &)),
	  this, SLOT(processError(const SignOn::Error &)));

  m_identity->storeCredentials(newInfo);
}

void StoreCredentialsProcessor::credentialsStored(quint32 id)
{
  m_identity.reset();

  emit stored(id);
}
