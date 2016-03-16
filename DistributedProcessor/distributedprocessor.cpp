

#include <stdio.h>
#include <conio.h>
#include <D3DX10.h>

#include <iostream>
#include "distributedprocessor.h"

CDistributedProcessor::CDistributedProcessor()
: m_bIsActive(false)
{

}
CDistributedProcessor::~CDistributedProcessor()
{
	m_bIsActive = false;
	if (m_pListenThread)
	{
		m_pListenThread->join();
		delete m_pListenThread;
		m_pListenThread = 0;
	}
}
void
CDistributedProcessor::Initialise()
{
	m_bIsActive = true;
	
	//Prompt user to enter port and IP address
	std::cout << "Enter receive port number on client: "; 
	std::cin >> m_sReceivePort;
	std::cout << "Enter Server's IP Address (#.#.#.#): "; 
	scanf_s("%i.%i.%i.%i", &m_tIP.iA, &m_tIP.iB, &m_tIP.iC, &m_tIP.iD);

	m_sSendPort = m_sReceivePort + 1;
	m_sRemotePort = SERVER_PORT; 

	//Open windows connection
	WSAStartup(0x0101, &m_tWindowsData);

	//Create UDP sockets
	DWORD dwNonBlocking = 1;
	m_sendSocket = socket(AF_INET, SOCK_DGRAM, 0); 
	m_receiveSocket = socket(AF_INET, SOCK_DGRAM, 0); 

	//Get Local host's name
	gethostname(m_cHostName, sizeof(m_cHostName));
	m_pHostInfo = gethostbyname(m_cHostName);
	if (m_pHostInfo == 0)
	{
		printf("Could not retrieve host data\n");
		closesocket(m_sendSocket);
		WSACleanup();
	}

	//Bind sockets to ports
	memset((void*)&m_tClient, '\0', sizeof(sockaddr_in));
	m_tClient.sin_family = AF_INET;
	m_tClient.sin_port = htons(m_sSendPort);

	m_tClient.sin_addr.S_un.S_un_b.s_b1 = m_pHostInfo->h_addr_list[0][0];
	m_tClient.sin_addr.S_un.S_un_b.s_b2 = m_pHostInfo->h_addr_list[0][1];
	m_tClient.sin_addr.S_un.S_un_b.s_b3 = m_pHostInfo->h_addr_list[0][2];
	m_tClient.sin_addr.S_un.S_un_b.s_b4 = m_pHostInfo->h_addr_list[0][3];

	//Bind socket to send port
	if (bind(m_sendSocket, (sockaddr*)&m_tClient, sizeof(sockaddr_in)) != 0)
	{
		printf("Bind failed on SendSocket\n");
	}

	//Clear server struct
	memset((void*)&m_tServer, '\0', sizeof(sockaddr_in));
	//Setup server information
	m_tServer.sin_family = AF_INET;
	m_tServer.sin_port = htons(m_sRemotePort); 

	//Set server address
	m_tServer.sin_addr.S_un.S_un_b.s_b1 = static_cast<unsigned char>(m_tIP.iA);
	m_tServer.sin_addr.S_un.S_un_b.s_b2 = static_cast<unsigned char>(m_tIP.iB);
	m_tServer.sin_addr.S_un.S_un_b.s_b3 = static_cast<unsigned char>(m_tIP.iC);
	m_tServer.sin_addr.S_un.S_un_b.s_b4 = static_cast<unsigned char>(m_tIP.iD);

	//Transmit the first message informing the server that we have connected
	TMessage tMessage;
	tMessage.eMessageType = MESSAGE_CONNECTED;
	sprintf_s(tMessage.cMessageBuffer, "Distributed Processor has connected\n");

	m_iServerLength = sizeof(sockaddr_in);
	if (sendto(m_sendSocket, reinterpret_cast<char*>(&tMessage), sizeof(tMessage), 0, (sockaddr*)&m_tServer, m_iServerLength) == -1)
	{
		printf("Could not send message\n");
		closesocket(m_sendSocket);
		WSACleanup();
	}
	//KEY AREA: Set socket timeout - otherwise program will hang waiting for recvfrom to receive data
	//int iTimeOut = 100;
	//setsockopt(m_receiveSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeOut, sizeof(iTimeOut));

	//Setup thread for listening
	m_pListenThread = new std::thread(&CDistributedProcessor::Listen, this);
}
void
CDistributedProcessor::Run()
{
	
}
void
CDistributedProcessor::Listen()
{
	int iBytesReceived = 0;
	char cMessageBuffer[sizeof(TMessage)];

	TMessage tEchoMessage;
	TMessage* pMessage = 0;

	while (m_bIsActive)
	{
		//Receive bytes from server
		iBytesReceived = recvfrom(m_sendSocket, cMessageBuffer, sizeof(TMessage), 0, (sockaddr*)&m_tClient, &m_iServerLength);
		int iError = WSAGetLastError();
		if (iBytesReceived > 0)
		{
			//Reinterpret the message
			pMessage = reinterpret_cast<TMessage*>(cMessageBuffer);
			if (pMessage != nullptr)
			{
				ProcessNetMessage(pMessage, &m_tClient);
			}
		}
	}
}
bool
CDistributedProcessor::IsActive() const
{
	return m_bIsActive;
}
void
CDistributedProcessor::ProcessNetMessage(TMessage* _pMessage, sockaddr_in* _pSourceClient)
{
	switch (_pMessage->eMessageType)
	{
	case MESSAGE_SENDGRASS:
	{
		ProcessGrassSection(_pMessage, 0, 0.0f);
		break;
	}
	case MESSAGE_SENDAI:
	{
		break;
	}
	default:
		break;
	}
}
/**
*
* CGrass class Processes a section of grass
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _iSection Section index for the grass
* @param _fDeltaTime Game time elapsed
*
*/
void
CDistributedProcessor::ProcessGrassSection(TMessage* _pMessage, int _iSection, float _fDeltaTime)
{
	fActiveGrassTimer += _fDeltaTime;
	TNetGrassData tGrassData;
	tGrassData.grassNormal = reinterpret_cast<D3DXVECTOR3*>(&_pMessage->cMessageBuffer);
	//KEY AREA: Distributed grass processing function
	if (m_pGrassData->iNumObstacles > 0)
	{
		int iStartingPosition = _iSection * m_pGrassData->iDivisionSize;
		for (int iBlade = 0; iBlade < m_pGrassData->iDivisionSize; ++iBlade)
		{
			int iCurrentVertex = iStartingPosition + iBlade;
			if (iCurrentVertex < m_pGrassData->iVertexCount)
			{				
				m_pGrassOutData[iCurrentVertex].x = sinf(fActiveGrassTimer) * 0.5f;
				m_pGrassOutData[iCurrentVertex].y = 1.0f;
				m_pGrassOutData[iCurrentVertex].z = 0.0f;
				D3DXVec3Normalize(&m_pGrassOutData[iCurrentVertex], &m_pGrassOutData[iCurrentVertex]);
			}
			if (m_pGrassOutData[iCurrentVertex].y < 0.5f)
			{
				m_pGrassOutData[iCurrentVertex].y = 0.5f;
			}
			}
		}
	}

	//Send final grass data back to server

}