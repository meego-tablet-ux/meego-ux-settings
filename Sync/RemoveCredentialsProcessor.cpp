/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include "RemoveCredentialsProcessor.hpp"

#include <SignOn/Identity>


RemoveCredentialsProcessor::RemoveCredentialsProcessor(QObject * parent)
  : SharedCredentialsProcessor(parent)
{
}

RemoveCredentialsProcessor::~RemoveCredentialsProcessor()
{
}

void RemoveCredentialsProcessor::newIdentity(QString const & provider)
{
  processError(SignOn::Error(SignOn::Error::IdentityNotFound,
			     provider + " not found"));
}

void RemoveCredentialsProcessor::processIdentity_i(
  SignOn::IdentityInfo const & /* info */)
{
  // Remove existing identity.

  connect(m_identity.data(), SIGNAL(removed()),
	  this, SLOT(credentialsRemoved()));
  connect(m_identity.data(), SIGNAL(error(const SignOn::Error &)),
	  this, SLOT(processError(const SignOn::Error &)));

  m_identity->remove();
}

void RemoveCredentialsProcessor::credentialsRemoved()
{
  m_identity.reset();

  emit removed();
}
