/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MEEGO_SHARED_CREDENTIALS_PROCESSOR_HPP
#define MEEGO_SHARED_CREDENTIALS_PROCESSOR_HPP

#include <SignOn/Identity>
#include <QObject>
#include <QScopedPointer>


/**
 * @class SharedCredentialsProcessor
 *
 * @brief Shared credentials processor strategy base class.
 *
 * Concrete shared credentials processor strategies implement this
 * interface.
 */
class SharedCredentialsProcessor : public QObject
{
  Q_OBJECT

public:

  /// Constructor.
  SharedCredentialsProcessor(QObject * parent = 0);

  /// Destructor.
  virtual ~SharedCredentialsProcessor();

  /// Method called when an identity is available for processing.
  void processIdentity(SignOn::IdentityInfo const & info);

  /// Method called when an identity corresponding to a SSO provider
  /// is not available.
  virtual void newIdentity(QString const & provider) = 0;

  // Check if we're in the wrong state to start a new credentials
  // processing operation.
  bool wrongState();

signals:

  /// Handling of shared credentials failed.
  void error(const SignOn::Error & error);

protected slots:

  /// Reset state and invoke our own error signal.
  void processError(const SignOn::Error & error);

private:

  // Disable copying.
  Q_DISABLE_COPY(SharedCredentialsProcessor)

  /// Template method called when an identity is available for
  /// processing.
  virtual void processIdentity_i(const SignOn::IdentityInfo & info) = 0;

  /// Template method called when checking for proper state when
  /// starting a new credentials processing operation.
  virtual bool operationPending() const;

  /// Clean up resources we're done with.
  void reset();

protected:

  /// Underlying identity object used to refer to the shared
  /// credentials.
  QScopedPointer<SignOn::Identity> m_identity;

};


#endif  /* MEEGO_SHARED_CREDENTIALS_PROCESSOR_HPP */
