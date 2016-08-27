
// Library Includes
#include <iostream>

// Local Includes
#include "grass.h"
#include "aihivemind.h"

// This Include
#include "network.h"

// Static Variables
Network* Network::s_pNetInstance = 0;

// Static Function Prototypes

// Implementation
DWORD WINAPI
ProcessGrass( LPVOID _pParameter )
{
	int x = 0;
	return 0;
}

Network::Network()
	: bHasSentMessage( false )
{

}

Network::~Network()
{
	m_bIsRunning = false;
	m_pListenThread->join();
	SAFEDELETE( m_pListenThread );

	m_pGrassData->CleanUp();
	SAFEDELETE( m_pGrassData );
}

bool
Network::Initialise( Grass* _pGrass, AIHiveMind* _pHivemind )
{
	printf( "\n============== STARTING NETWORK CONNECTION ==============\n" );

	CreateServer();
	m_pListenThread = new std::thread( &Network::Listen, this );

	m_pGrass = _pGrass;
	m_pHivemind = _pHivemind;

	//Setup grass data
	m_pGrassData = new TNetGrassData();
	m_pGrassData->iVertexCount = m_pGrass->GetVertexCount();
	m_pGrassData->iDivisionSize = m_pGrass->GetDimensionSize();
	m_pGrassData->grassPos = new Math::Vector3[ m_pGrassData->iVertexCount ];
	m_pGrassData->grassNormal = new Math::Vector3[ m_pGrassData->iVertexCount ];
	for( int iGrassBlade = 0; iGrassBlade < m_pGrassData->iVertexCount; ++iGrassBlade )
	{
		TVertex* pCurrentBlade = m_pGrass->GetVertexData( iGrassBlade );
		m_pGrassData->grassPos[ iGrassBlade ] = pCurrentBlade->pos;
		m_pGrassData->grassNormal[ iGrassBlade ] = pCurrentBlade->normal;
	}

	return true;
}

void
Network::CreateServer()
{
	WSADATA wsaData;
	//Start WSA
	printf( "-\tStarting WSA\n" );
	if( WSAStartup( 0x0101, &wsaData ) != 0 )
	{
		printf( "COULD NOT START WSA\n" );
	}
	//Create datagram socket
	printf( "-\tCreating datagram socket\n" );
	m_sServerSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	if( m_sServerSocket == INVALID_SOCKET )
	{
		printf( "FAILED TO CREATE SOCKET\n" );
		WSACleanup();
	}

	//Set server information
	ZeroMemory( &m_tServer, sizeof( sockaddr_in ) );
	m_tServer.sin_family = AF_INET;
	m_tServer.sin_port = htons( SERVER_PORT );

	//Set address automatically
	char* pcHostName = new char[ 64 ];
	gethostname( pcHostName, 64 );

	HOSTENT* pHostData = gethostbyname( pcHostName );
	m_tServer.sin_addr.S_un.S_un_b.s_b1 = static_cast<unsigned char>( pHostData->h_addr_list[ 0 ][ 0 ] );
	m_tServer.sin_addr.S_un.S_un_b.s_b2 = static_cast<unsigned char>( pHostData->h_addr_list[ 0 ][ 1 ] );
	m_tServer.sin_addr.S_un.S_un_b.s_b3 = static_cast<unsigned char>( pHostData->h_addr_list[ 0 ][ 2 ] );
	m_tServer.sin_addr.S_un.S_un_b.s_b4 = static_cast<unsigned char>( pHostData->h_addr_list[ 0 ][ 3 ] );

	//Bind address to socket
	if( bind( m_sServerSocket, ( struct sockaddr* )&m_tServer, sizeof( struct sockaddr_in ) ) == -1 )
	{
		printf( "-\tERROR: Could not bind name to socket\n" );
		closesocket( m_sServerSocket );
		WSACleanup();
		exit( 0 );
	}

	//Output the IP address
	printf( "-\tServer started at: %u.%u.%u.%u\n", m_tServer.sin_addr.S_un.S_un_b.s_b1,
		m_tServer.sin_addr.S_un.S_un_b.s_b2,
		m_tServer.sin_addr.S_un.S_un_b.s_b3,
		m_tServer.sin_addr.S_un.S_un_b.s_b4 );

	m_iClientLength = static_cast<int>( sizeof( sockaddr_in ) );
	//KEY AREA: Set socket timeout - otherwise program will hang waiting for recvfrom to receive data
	int iTimeOut = 100;
	setsockopt( m_sServerSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&iTimeOut, sizeof( iTimeOut ) );

	printf( "============== NETWORK CONNECTION SUCCESSFUL ==============\n\n" );
	SAFEDELETEARRAY( pcHostName );
}

void
Network::Process( float _fDeltaTime )
{

}

//==================================================================
//		SERVER LISTENING LOOP - this runs on m_pListenThread
//==================================================================
void
Network::Listen()
{
	int iBytesReceived = 0;
	char cMessageBuffer[ sizeof( TMessage ) ];

	TMessage tEchoMessage;
	TMessage* pMessage = 0;
	m_bIsRunning = true;

	while( m_bIsRunning )
	{
		//Receive bytes from client
		iBytesReceived = recvfrom( m_sServerSocket, cMessageBuffer, sizeof( TMessage ), 0, (sockaddr*)&m_tClient, &m_iClientLength );
		if( iBytesReceived > 0 )
		{
			//Reinterpret the message
			pMessage = reinterpret_cast<TMessage*>( cMessageBuffer );
			if( pMessage != nullptr )
			{
				ProcessNetMessage( pMessage, &m_tClient );
			}
		}
	}
}

void
Network::SendData( void* _pData, EMessageType _eMessageType, size_t _iSize, sockaddr_in* _pClient )
{
	TMessage tMessage;
	tMessage.eMessageType = _eMessageType;
	memcpy( tMessage.cMessageBuffer, _pData, _iSize );
	if( ( sendto( m_sServerSocket, reinterpret_cast<char*>( &tMessage ), sizeof( TMessage ), 0, (sockaddr*)_pClient, m_iClientLength ) ) == -1 )
	{
		int iError = WSAGetLastError();
		printf( "== Server message failed to send (Error code: %i) ==\n", iError );
	}
}

void
Network::ProcessNetMessage( TMessage* _pMessage, sockaddr_in* _pSourceClient )
{
	switch( _pMessage->eMessageType )
	{
	case MESSAGE_CONNECTED:
	{
		printf( "-\tDistributed Client connected!\n" );
		m_vecClients.push_back( new TClientData( _pSourceClient ) );
		break;
	}
	case MESSAGE_RECEIVEGRASS:
	{
		printf( "Received grass!\n" );
		break;
	}
	case MESSAGE_RECEIVEAI:
	{
		break;
	}
	default:
		printf( "%s\n", _pMessage->cMessageBuffer );
		break;
	}
}

void
Network::ProcessGrass( float _fDeltaTime )
{
	if( m_vecClients.size() > 0 )
	{
		char* cMessage = reinterpret_cast<char*>( m_pGrassData->grassNormal );
		SendData( cMessage, MESSAGE_SENDGRASS, sizeof( m_pGrassData->grassNormal ), m_vecClients[ 0 ]->pAddress );
	}
}

void
Network::ProcessAI( float _fDeltaTime )
{

}

void
Network::SendGrassData( Grass* _pGrass, std::vector<RenderEntity*>* _pCollisionObjects )
{
	//Pack obstacle data into contiguous blocks
	m_pGrassData->iNumObstacles = static_cast<int>( _pCollisionObjects->size() );
	if( m_pGrassData->pObstacleData != nullptr )
	{
		SAFEDELETEARRAY( m_pGrassData->pObstacleData );
	}

	m_pGrassData->pObstacleData = new Math::Vector4[ m_pGrassData->iNumObstacles ];
	for( int iObstacle = 0; iObstacle < m_pGrassData->iNumObstacles; ++iObstacle )
	{
		m_pGrassData->pObstacleData[ iObstacle ].x = ( *_pCollisionObjects )[ iObstacle ]->GetPosition().x;
		m_pGrassData->pObstacleData[ iObstacle ].y = ( *_pCollisionObjects )[ iObstacle ]->GetPosition().y;
		m_pGrassData->pObstacleData[ iObstacle ].z = ( *_pCollisionObjects )[ iObstacle ]->GetPosition().z;
		m_pGrassData->pObstacleData[ iObstacle ].w = ( *_pCollisionObjects )[ iObstacle ]->GetRadius();
	}

	int iDataSize = ( sizeof( m_pGrassData->grassPos ) * m_pGrassData->iVertexCount ) +
		sizeof( m_pGrassData->pObstacleData ) * m_pGrassData->iVertexCount +
		sizeof( m_pGrassData->grassNormal ) * m_pGrassData->iVertexCount;
	printf( "Packet size: %i\n", iDataSize );
}

//==================================================================
//		SINGLETON METHODS
//==================================================================
Network*
Network::GetInstance()
{
	if( s_pNetInstance == 0 )
	{
		s_pNetInstance = new Network();
	}

	return ( s_pNetInstance );
}

void
Network::DestroyInstance()
{
	delete s_pNetInstance;
	s_pNetInstance = 0;
}