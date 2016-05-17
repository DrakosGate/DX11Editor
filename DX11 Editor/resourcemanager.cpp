
// Library Includes
//#include <D3DX11.h>
#include <iostream>

// Local Includes
#include "model.h"
#include "animatedmodel.h"
#include "entitymanager.h"
#include "scenehierarchy.h"

// This Include
#include "resourcemanager.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CResourceManager class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CResourceManager::CResourceManager()
	: m_pTextureArray( 0 )
{

}
/**
*
* CResourceManager class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CResourceManager::~CResourceManager()
{
	for( unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture )
	{
		ReleaseCOM( m_TexturePool[ iTexture ]->pTexture );
		delete m_TexturePool[ iTexture ];
		m_TexturePool[ iTexture ] = 0;
	}
	m_TexturePool.clear();

	std::map<std::string, Model*>::iterator modelIter;
	//std::map<std::string, CAnimatedModel*>::iterator animIter = m_mapAnimations.begin();
	for( modelIter = m_mapModels.begin(); modelIter != m_mapModels.end(); ++modelIter )
	{
		delete modelIter->second;
		modelIter->second = 0;
	}
	//for( animIter = m_mapAnimations.begin(); animIter != m_mapAnimations.end(); ++animIter )
	//{
	//	delete animIter->second;
	//	animIter->second = 0;
	//}
}
/**
*
* CResourceManager class Initialise Loads resources from Scene Hierarchy
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcResourceFilename Filename containing level resources
*
*/
void
CResourceManager::Initialise( ID3D11Device* _pDevice, CEntityManager* _pEntityManager, CSceneHierarchy* _pSceneHierarchy )
{
	int iMaxMessageSize = 128;
	char pcBuffer[ 128 ];
	//Loop through models
	printf( "\n  == LOADING MODELS\n" );
	for( unsigned int iModel = 0; iModel < _pSceneHierarchy->GetResourceCount( RESOURCE_MODEL ); ++iModel )
	{
		std::string sModelName = _pSceneHierarchy->GetResourceName( RESOURCE_MODEL, iModel );
		std::string sModelFilename = _pSceneHierarchy->GetResourceFilename( RESOURCE_MODEL, sModelName );
		sprintf_s( pcBuffer, iMaxMessageSize, "%s", sModelFilename.c_str() );

		//Load model from file
		Model* pNewModel = new Model();
		pNewModel->Initialise();
		pNewModel->LoadFromOBJ( _pDevice, 1.0f, pcBuffer, Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f ) );

		//Add to model map
		printf( "    = Model successfully loaded from %s\n", pcBuffer );
		m_mapModels[ sModelName ] = pNewModel;
	}

	//Loop through textures
	printf( "\n  == LOADING TEXTURES\n" );
	for( unsigned int iTexture = 0; iTexture < _pSceneHierarchy->GetResourceCount( RESOURCE_TEXTURE ); ++iTexture )
	{
		std::string sTextureName = _pSceneHierarchy->GetResourceName( RESOURCE_TEXTURE, iTexture );
		std::string sTextureFilename = _pSceneHierarchy->GetResourceFilename( RESOURCE_TEXTURE, sTextureName );
		sprintf_s( pcBuffer, iMaxMessageSize, "%s", sTextureFilename.c_str() );

		//Load new textures from file
		ID3D11ShaderResourceView* pNewTexture;
		HRESULT hTextureResult;// = D3DX11CreateShaderResourceViewFromFileA(	_pDevice,
		//																	pcBuffer,
		//																	NULL,
		//																	NULL,
		//																	&pNewTexture,
		//																	NULL);
		if( hTextureResult == S_OK )
		{
			printf( "    = Texture successfully loaded from %s\n", pcBuffer );
		}
		else
		{
			printf( "!!!  TEXTURE LOAD FAILED: %s  !!!\n", pcBuffer );
		}
		TTexturePoolData* pNewPoolEntry = new TTexturePoolData;
		pNewPoolEntry->pTexture = pNewTexture;
		pNewPoolEntry->sName = sTextureName;
		m_TexturePool.push_back( pNewPoolEntry );
	}

	//Pass prefab types to entity manager
	for( unsigned int iPrefab = 0; iPrefab < _pSceneHierarchy->GetPrefabCount(); ++iPrefab )
	{
		TPrefabDefinition* pPrefab = _pSceneHierarchy->GetPrefabDefinition( iPrefab );
		if( pPrefab )
		{
			AddPrefabToEntityManager( _pEntityManager, pPrefab );
		}
	}
}
/**
*
* CResourceManager class Adds a prefab to the Entity managers prefabs
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the new texture
*
*/
void
CResourceManager::AddPrefabToEntityManager( CEntityManager* _pEntityManager, TPrefabDefinition* _pPrefab )
{
	EAIType eAIType = AI_INVALID;
	if( _pPrefab->sAIType == "human" )
	{
		eAIType = AI_HUMAN;
	}
	else if( _pPrefab->sAIType == "chicken" )
	{
		eAIType = AI_CHICKEN;
	}
	TPrefabOptions* pNewPrefabOptions = new TPrefabOptions( _pPrefab->sName,
		m_mapModels[ _pPrefab->sName ],
		GetTexture( _pPrefab->sTexture ),
		Math::Vector3( _pPrefab->vecScale[ 0 ], _pPrefab->vecScale[ 1 ], _pPrefab->vecScale[ 2 ] ),
		AI_CHICKEN,
		_pPrefab->bIsAnimated,
		_pPrefab->bIsStatic );
	//Add prefab children
	for( unsigned int iChild = 0; iChild < _pPrefab->vecChildren.size(); ++iChild )
	{
		pNewPrefabOptions->vecChildren.push_back( _pPrefab->vecChildren[ iChild ] );
	}
	_pEntityManager->AddPrefab( pNewPrefabOptions );
}
/**
*
* CResourceManager class Creates a texture file from an array of data
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the new texture
*
*/
ID3D11ShaderResourceView*
CResourceManager::CreateTextureFromData( ID3D11Device* _pDevice, unsigned char* _pcData, std::string& _sTextureString, int _iWidth, int _iHeight )
{
	ID3D11Texture2D* pNewTexture;
	TTexturePoolData* pNewPoolData = new TTexturePoolData();

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory( &textureDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
	ZeroMemory( &resourceData, sizeof( D3D11_SUBRESOURCE_DATA ) );

	textureDesc.Width = _iWidth;
	textureDesc.Height = _iHeight;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;
	textureDesc.ArraySize = 1;

	resourceData.pSysMem = _pcData;
	resourceData.SysMemPitch = _iWidth * 4 * sizeof( unsigned char );
	resourceData.SysMemSlicePitch = _iWidth * _iHeight * 4 * sizeof( unsigned char );

	HRESULT hResult = _pDevice->CreateTexture2D( &textureDesc, &resourceData, &pNewTexture );
	HRCheck( hResult, L"Could not create texture from data" );
	hResult = _pDevice->CreateShaderResourceView( pNewTexture, NULL, &pNewPoolData->pTexture );
	HRCheck( hResult, L"Could not bind custom texture to shader resource view" );
	pNewPoolData->sName = _sTextureString;
	m_TexturePool.push_back( pNewPoolData );
	pNewTexture->Release();

	return pNewPoolData->pTexture;
}
/**
*
* CResourceManager class GetModel
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pModelName Name of model
* @return Returns Model specified
*
*/
Model*
CResourceManager::GetModel( std::string& _pModelName ) const
{
	return ( m_mapModels.find( _pModelName )->second );
}
/**
*
* CResourceManager class GetAnimatedModel
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcAnimatedModelName Name of model
* @return Returns Model specified
*
*/
CAnimatedModel*
CResourceManager::GetAnimatedModel( std::string& _pcAnimatedModelName ) const
{
	std::string sKey = _pcAnimatedModelName;
	return ( m_mapAnimations.find( sKey )->second );
}
/**
*
* CResourceManager class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcTextureName Name of texture
* @return Returns Texture specified
*
*/
ID3D11ShaderResourceView*
CResourceManager::GetTexture( std::string& _pcTextureName ) const
{
	//Loop through texture vector and return texture matching this name
	ID3D11ShaderResourceView* pTexture = 0;
	for( unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture )
	{
		if( strcmp( m_TexturePool[ iTexture ]->sName.c_str(), _pcTextureName.c_str() ) == 0 )
		{
			pTexture = m_TexturePool[ iTexture ]->pTexture;
			break;
		}
	}
	if( pTexture == 0 )
	{
		Error( L"Could not find texture in pool!!" );
	}
	return ( pTexture );
}
/**
*
* CResourceManager class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcTextureName Name of texture
* @return Returns Index of the texture specified
*
*/
int
CResourceManager::GetTextureID( std::string& _pcTextureName ) const
{
	//Loop through texture vector and return texture matching this name
	int iTextureID = -1;
	for( unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture )
	{
		if( strcmp( m_TexturePool[ iTexture ]->sName.c_str(), _pcTextureName.c_str() ) == 0 )
		{
			iTextureID = iTexture;
			break;
		}
	}
	if( iTextureID == -1 )
	{
		Error( L"Could not find texture in pool!!" );
	}
	return ( iTextureID );
}
/**
*
* CResourceManager class GetTextureID
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pTexture Pointer to the texture
* @return Returns Index of the texture specified
*
*/
int
CResourceManager::GetTextureID( ID3D11ShaderResourceView* _pTexture ) const
{
	//Loop through texture vector and return texture matching this name
	int iTextureID = -1;
	for( unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture )
	{
		if( m_TexturePool[ iTexture ]->pTexture == _pTexture )
		{
			iTextureID = iTexture;
			break;
		}
	}
	if( iTextureID == -1 )
	{
		Error( L"Could not find texture in pool!!" );
	}
	return ( iTextureID );
}
/**
*
* CResourceManager class Compacts texture data into buffer and sends the data to the shader
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevContext Pointer to the device context
*
*/
void
CResourceManager::SendTextureDataToShader( ID3D11DeviceContext* _pDevContext )
{
	int iTextureCount = m_TexturePool.size();
	m_pTextureArray = new ID3D11ShaderResourceView*[ iTextureCount ];
	for( int iTexture = 0; iTexture < iTextureCount; ++iTexture )
	{
		m_pTextureArray[ iTextureCount - 1 - iTexture ] = m_TexturePool[ iTextureCount - iTexture - 1 ]->pTexture;
	}
	_pDevContext->PSSetShaderResources( 0, iTextureCount, m_pTextureArray );

	delete[] m_pTextureArray;
	m_pTextureArray = 0;
}