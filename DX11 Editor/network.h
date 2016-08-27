//
//  File Name   :   network.h
//  Description :   Class of Network
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
#include "renderentity.h"
#include "networkdefines.h"

// Types

// Constants

// Prototypes
class Grass;
class AIHiveMind;

class Network	//			SINGLETON
{
	//Member functions
public:
	virtual ~Network();

	bool Initialise( Grass* _pGrass, AIHiveMind* _pHivemind );
	void CreateServer();

	void Process( float _fDeltaTime );
	void Listen();
	void SendData( void* _pData, EMessageType _eMessageType, size_t _iSize, sockaddr_in* _pClient );

	//Distributed functions
	void ProcessNetMessage( TMessage* _pMessage, sockaddr_in* _pSourceClient );
	void ProcessGrass( float _fDeltaTime );
	void ProcessAI( float _fDeltaTime );
	void SendGrassData( Grass* _pGrass, std::vector< RenderEntity* >* _pCollisionObjects );

	//Singleton Methods
	static Network* GetInstance();
	static void DestroyInstance();

private:
	Network();
	Network( const Network& _krInstanceToCopy );
	const Network& operator =( const Network& _krInstanceToCopy );

	//Member variables
protected:
	static Network* s_pNetInstance;
	std::vector<TClientData*> m_vecClients;
	TNetGrassData* m_pGrassData;

	SOCKET m_sServerSocket;
	struct sockaddr_in m_tServer;
	struct sockaddr_in m_tClient;
	int m_iClientLength;
	bool bHasSentMessage;

	std::thread* m_pListenThread;

	Grass* m_pGrass;
	AIHiveMind* m_pHivemind;
	bool m_bIsRunning;
};

#endif //__NETWORK_H__