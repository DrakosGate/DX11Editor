
#ifndef __NETDEFINES_H__
#define __NETDEFINES_H__

#include "defines.h"

// Types
#define SERVER_PORT 56000
#define MESSAGE_BUFFER_SIZE 500000

// Different message types being send
enum EMessageType
{
	MESSAGE_INVALID = -1,
	MESSAGE_MESSAGE,
	MESSAGE_CONNECTED,
	MESSAGE_DISCONNECTED,

	MESSAGE_SENDGRASS,
	MESSAGE_RECEIVEGRASS,

	MESSAGE_SENDAI,
	MESSAGE_RECEIVEAI,

	MESSAGE_MAX
};
//IP Address structure
struct TIPAddress
{
	TIPAddress()
	{
		iA = 0;
		iB = 0;
		iC = 0;
		iD = 0;
	}
	int iA;
	int iB;
	int iC;
	int iD;
};
//Default message information
struct TMessage
{
	TMessage()
	{
		eMessageType = MESSAGE_INVALID;
	}
	EMessageType eMessageType;
	char cMessageBuffer[MESSAGE_BUFFER_SIZE];
};
struct TClientData
{
	TClientData(){};
	TClientData(sockaddr_in* _pAddress)
	{
		pAddress = _pAddress;
	}
	sockaddr_in* pAddress;
};
struct TNetGrassData
{
	TNetGrassData()
	{
		grassPos = 0;
		grassNormal = 0;
		pObstacleData = 0;
		iDivisionSize = 0;
		iVertexCount = 0;
		iNumObstacles = 0;
	}
	void CleanUp()
	{
		SAFEDELETEARRAY(grassPos);
		SAFEDELETEARRAY(grassNormal);
		SAFEDELETEARRAY(pObstacleData);
	}
	Math::Vector3* grassPos;
	Math::Vector3* grassNormal;
	Math::Vector4* pObstacleData; //3x position data + 1x radius
	int iDivisionSize;
	int iVertexCount;
	int iNumObstacles;
};


#endif //NETDEFINES