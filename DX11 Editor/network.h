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
#include "networkdefines.h"

// Types

// Constants

// Prototypes
class CGrass;
class CAIHiveMind;

class CNetwork	//			SINGLETON
{
	//Member functions
public:
	virtual ~CNetwork();

	bool Initialise(CGrass* _pGrass, CAIHiveMind* _pHivemind);
	void CreateServer();

	void Process(float _fDeltaTime);
	void Listen();
	void SendData(void* _pData, EMessageType _eMessageType, size_t _iSize, sockaddr_in* _pClient);

	//Distributed functions
	void ProcessNetMessage(TMessage* _pMessage, sockaddr_in* _pSourceClient);
	void ProcessGrass(float _fDeltaTime);
	void ProcessAI(float _fDeltaTime);
	void SendGrassData(CGrass* _pGrass, std::vector<CRenderEntity*>* _pCollisionObjects);

	//Singleton Methods
	static CNetwork* GetInstance();
	static void DestroyInstance();

private:
	CNetwork();
	CNetwork(const CNetwork& _krInstanceToCopy);
	const CNetwork& operator =(const CNetwork& _krInstanceToCopy);

	//Member variables
protected:
	static CNetwork* s_pNetInstance;
	std::vector<TClientData*> m_vecClients;
	TNetGrassData* m_pGrassData;
	
	SOCKET m_sServerSocket;
	struct sockaddr_in m_tServer;
	struct sockaddr_in m_tClient;
	int m_iClientLength;
	bool bHasSentMessage;

	std::thread* m_pListenThread;

	CGrass* m_pGrass;
	CAIHiveMind* m_pHivemind;
	bool m_bIsRunning;
};

#endif //__NETWORK_H__