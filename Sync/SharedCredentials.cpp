/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include "SharedCredentials.hpp"

#include <QDebug>


SharedCredentials::SharedCredentials(QObject * parent)
  : QObject(parent)
  , m_provider()
  , m_processor(0)
  , m_service(0)
{
  m_service = new SignOn::AuthService(this);

  // m_service->dumpObjectInfo();

  connect(m_service,
	  SIGNAL(identities(const QList<SignOn::IdentityInfo> &)),
	  this,
	  SLOT(identities(const QList<SignOn::IdentityInfo> &)));

  qRegisterMetaType<SignOn::IdentityInfo>("SignOn::IdentityInfo");
}

void SharedCredentials::processCredentials(
  SharedCredentialsProcessor * processor,
  QString provider)
{
  if (m_processor != 0 || processor == 0 || processor->wrongState()) {
    qDebug() << "WARNING: Credentials processing is pending.";
    return;
  }

  m_processor = processor;
  m_provider = provider;

  // query methods and mechanisms merely for debugging
  m_service->queryMethods();

  // request list of existing identies, further processing in
  // identities() slot
  m_service->queryIdentities();
}

void SharedCredentials::identities(
  const QList<SignOn::IdentityInfo> &identityList)
{
  for (int i = 0; i < identityList.size(); ++i) {
    if (identityList.at(i).caption() == m_provider) {
      m_processor->processIdentity(identityList.at(i));
      m_processor = 0;
      return;
    }
  }

  m_processor->newIdentity(m_provider);
  m_processor = 0;
}
