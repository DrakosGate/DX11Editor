//
//  File Name   :   network.h
//  Description :   Class of CNetwork
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __NETWORK_H__
#define __NETWORK_H__

// Library Includes
#include <WinSock.h>
#include <thread>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes

class CNetwork
{
	//Member functions
public:
	CNetwork();
	virtual ~CNetwork();

	bool Initialise();
	void CreateServer();

	void Listen();

private:
	CNetwork(const CNetwork& _krInstanceToCopy);
	const CNetwork& operator =(const CNetwork& _krInstanceToCopy);

	//Member variables
protected:
	SOCKET m_sSocket;
	struct sockaddr_in m_tServer;
};

#endif //__NETWORK_H__