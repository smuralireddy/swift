/*
 * Copyright (c) 2010 Remko Tronçon
 * Licensed under the GNU General Public License v3.
 * See Documentation/Licenses/GPLv3.txt for more information.
 */

#pragma once

#include <Swiften/Base/boost_bsignals.h>
#include <boost/shared_ptr.hpp>

#include <Swiften/Base/SafeByteArray.h>
#include <Swiften/TLS/Certificate.h>
#include <Swiften/TLS/CertificateWithKey.h>
#include <Swiften/TLS/CertificateVerificationError.h>
#include <Swiften/TLS/TLSError.h>

namespace Swift {

	class TLSContext {
		public:
			virtual ~TLSContext();

			virtual void connect() = 0;

			virtual bool setClientCertificate(CertificateWithKey::ref cert) = 0;

			virtual void handleDataFromNetwork(const SafeByteArray&) = 0;
			virtual void handleDataFromApplication(const SafeByteArray&) = 0;

			virtual Certificate::ref getPeerCertificate() const = 0;
			virtual CertificateVerificationError::ref getPeerCertificateVerificationError() const = 0;

			virtual ByteArray getFinishMessage() const = 0;

		public:
			boost::signal<void (const SafeByteArray&)> onDataForNetwork;
			boost::signal<void (const SafeByteArray&)> onDataForApplication;
			boost::signal<void (boost::shared_ptr<TLSError>)> onError;
			boost::signal<void ()> onConnected;
	};
}
