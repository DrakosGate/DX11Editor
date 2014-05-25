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
	printf("============== STARTING NETWORK CONNECTION ==============\n");
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
	SOCKET tSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (tSocket == INVALID_SOCKET)
	{
		printf("FAILED TO CREATE SOCKET\n");
		WSACleanup();
	}
	sockaddr_in tServerData;
	ZeroMemory(&tServerData, sizeof(sockaddr_in));

	tServerData.sin_family = AF_INET;
	tServerData.sin_port = htons(27890);

	//Set address automatically
	char* pcHostName = new char[64];
	gethostname(pcHostName, 64);
	HOSTENT* pHostData = gethostbyname(pcHostName);
}
void
CNetwork::CreateClient()
{

}
void
CNetwork::JoinServer()
{
	
}