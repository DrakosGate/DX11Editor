//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   network.cpp
//  Description :   Code for class CNetwork
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <iostream>

// Local Includes

// This Include
#include "network.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CNetwork class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CNetwork::CNetwork()
{

}

/**
*
* CNetwork class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CNetwork::~CNetwork()
{

}

/**
*
* CNetwork class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns true
*
*/
bool
CNetwork::Initialise()
{
	printf("\n============== STARTING NETWORK CONNECTION ==============\n");
	return true;
}
void
CNetwork::CreateServer()
{
	WSADATA wsaData;
	//Start WSA
	printf("-\tStarting WSA\n");
	if (WSAStartup(0x0101, &wsaData) != 0)
	{
		printf("COULD NOT START WSA\n");
	}
	//Create datagram socket
	printf("-\tCreating datagram socket\n");
	m_sSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_sSocket == INVALID_SOCKET)
	{
		printf("FAILED TO CREATE SOCKET\n");
		WSACleanup();
	}

	//Set server information
	ZeroMemory(&m_tServer, sizeof(sockaddr_in));
	m_tServer.sin_family = AF_INET;
	m_tServer.sin_port = htons(SERVER_PORT);

	//Set address automatically
	char* pcHostName = new char[64];
	gethostname(pcHostName, 64);
	
	HOSTENT* pHostData = gethostbyname(pcHostName);
	m_tServer.sin_addr.S_un.S_un_b.s_b1 = static_cast<unsigned char>(pHostData->h_addr_list[0][0]);
	m_tServer.sin_addr.S_un.S_un_b.s_b2 = static_cast<unsigned char>(pHostData->h_addr_list[0][1]);
	m_tServer.sin_addr.S_un.S_un_b.s_b3 = static_cast<unsigned char>(pHostData->h_addr_list[0][2]);
	m_tServer.sin_addr.S_un.S_un_b.s_b4 = static_cast<unsigned char>(pHostData->h_addr_list[0][3]);

	//Bind address to socket
	if (bind(m_sSocket, (struct sockaddr*)&m_tServer, sizeof(struct sockaddr_in)) == -1)
	{
		printf("-\tERROR: Could not bind name to socket\n");
		closesocket(m_sSocket);
		WSACleanup();
		exit(0);
	}

	//Output the IP address
	printf("-\tServer started at: %u.%u.%u.%u\n",	m_tServer.sin_addr.S_un.S_un_b.s_b1,
													m_tServer.sin_addr.S_un.S_un_b.s_b2,
													m_tServer.sin_addr.S_un.S_un_b.s_b3,
													m_tServer.sin_addr.S_un.S_un_b.s_b4);

	printf("============== NETWORK CONNECTION SUCCESSFUL ==============\n\n");
	SAFEDELETEARRAY(pcHostName);
}
void 
CNetwork::Listen()
{

}