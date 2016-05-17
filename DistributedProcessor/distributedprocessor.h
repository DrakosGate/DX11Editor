

#ifndef __DISTRIBUTEDPROCESSOR_H__
#define __DISTRIBUTEDPROCESSOR_H__

#include <winsock.h>
#include <Windows.h>
#include <thread>

#include "../DX11 Editor/networkdefines.h"


class CDistributedProcessor
{
	//Member variables
public:
	CDistributedProcessor();
	~CDistributedProcessor();

	void Initialise();
	void Run();
	void Listen();
	bool IsActive() const;

	//Distributed Processes
	void ProcessNetMessage( TMessage* _pMessage, sockaddr_in* _pSourceClient );
	void ProcessGrassSection( TMessage* _pMessage, int _iSection, float _fDeltaTime );

	//Member functions
private:
	//General Processing
	bool m_bIsActive;
	TNetGrassData* m_pGrassData;
	Math::Vector3* m_pGrassOutData;
	float fActiveGrassTimer;

	//Networking variables
	TIPAddress m_tIP;
	WSADATA m_tWindowsData;
	unsigned short m_sReceivePort;
	unsigned short m_sSendPort;
	unsigned short m_sRemotePort;
	SOCKET m_sendSocket;
	SOCKET m_receiveSocket;
	struct hostent* m_pHostInfo;
	char m_cHostName[ 256 ];
	int m_iServerLength;

	//Sockets
	struct sockaddr_in m_tServer;
	struct sockaddr_in m_tClient;

	std::thread* m_pListenThread;

};


#endif //DISTRIBUTEDPROCESSOR