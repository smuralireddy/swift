/*
 * Copyright (c) 2011 Soren Dreijer
 * Licensed under the simplified BSD license.
 * See Documentation/Licenses/BSD-simplified.txt for more information.
 */

#pragma once

#include "Swiften/Base/boost_bsignals.h"

#include "Swiften/TLS/TLSContext.h"
#include "Swiften/TLS/Schannel/SchannelUtil.h"
#include <Swiften/TLS/CertificateWithKey.h>
#include "Swiften/Base/ByteArray.h"

#define SECURITY_WIN32
#include <Windows.h>
#include <Schannel.h>
#include <security.h>
#include <schnlsp.h>

#include <boost/noncopyable.hpp>

namespace Swift 
{	
	class SchannelContext : public TLSContext, boost::noncopyable 
	{
	public:
		typedef boost::shared_ptr<SchannelContext> sp_t;

	public:
		SchannelContext();

		~SchannelContext();

		//
		// TLSContext
		//
		virtual void	connect();
		virtual bool	setClientCertificate(CertificateWithKey::ref cert);

		virtual void	handleDataFromNetwork(const SafeByteArray& data);
		virtual void	handleDataFromApplication(const SafeByteArray& data);

		virtual Certificate::ref getPeerCertificate() const;
		virtual CertificateVerificationError::ref getPeerCertificateVerificationError() const;

		virtual ByteArray getFinishMessage() const;

	private:
		void			determineStreamSizes();
		void			continueHandshake(const SafeByteArray& data);
		void			indicateError();
		void			handleCertError(SECURITY_STATUS status) ;

		void			sendDataOnNetwork(const void* pData, size_t dataSize);
		void			forwardDataToApplication(const void* pData, size_t dataSize);

		void			decryptAndProcessData(const SafeByteArray& data);
		void			encryptAndSendData(const SafeByteArray& data);

		void			appendNewData(const SafeByteArray& data);
		SECURITY_STATUS validateServerCertificate();

	private:
		enum SchannelState
		{
			Start,
			Connecting,
			Connected,
			Error

		};

		SchannelState		m_state;
		boost::optional<CertificateVerificationError> m_verificationError;

		ULONG				m_secContext;
		ScopedCredHandle	m_credHandle;
		ScopedCtxtHandle	m_ctxtHandle;
		DWORD				m_ctxtFlags;
		SecPkgContext_StreamSizes m_streamSizes;

		std::vector<char>	m_receivedData;

		HCERTSTORE		m_my_cert_store;
		std::string		m_cert_store_name;
		std::string		m_cert_name;
	};
}
