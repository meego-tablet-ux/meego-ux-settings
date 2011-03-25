/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */


#include "SharedCredentialsProcessor.hpp"

#include <SignOn/Identity>


SharedCredentialsProcessor::SharedCredentialsProcessor(QObject * parent)
  : QObject(parent)
  , m_identity()
{
}

SharedCredentialsProcessor::~SharedCredentialsProcessor()
{
}

void SharedCredentialsProcessor::processIdentity(
  SignOn::IdentityInfo const & info)
{
  m_identity.reset(SignOn::Identity::existingIdentity(info.id()));
  if (m_identity.isNull()) {
    this->processError(SignOn::Error(SignOn::Error::IdentityNotFound,
				      "Identity not found!"));
  } else {
    // Execute credentials handling strategy-specific processing.
    this->processIdentity_i(info);
  }
}

bool SharedCredentialsProcessor::wrongState()
{
  bool const pending = (!m_identity.isNull() || this->operationPending());

  if (pending) {
    error(SignOn::Error(SignOn::Error::WrongState,
			"SharedCredentials operation in progress"));
  }

  return pending;
}

bool SharedCredentialsProcessor::operationPending() const
{
  return false;
}

void SharedCredentialsProcessor::processError(const SignOn::Error & err)
{
  m_identity.reset();

  emit error(err);
}
