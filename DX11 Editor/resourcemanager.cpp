
// Library Includes
#include <d3d11.h>
#include <iostream>
#include "lodepng.h"

// Local Includes
#include "model.h"
#include "animatedmodel.h"
#include "entitymanager.h"
#include "scenehierarchy.h"
#include "datastructures.h"

// This Include
#include "resourcemanager.h"

// Static Variables

// Static Function Prototypes

// Implementation

ResourceManager::ResourceManager()
	: m_pTextureArray( 0 )
{

}

ResourceManager::~ResourceManager()
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

void
ResourceManager::Initialise( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, EntityManager* _pEntityManager, SceneHierarchy* _pSceneHierarchy )
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
		TTexturePoolData* pNewPoolEntry = new TTexturePoolData;
		
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;

		pNewPoolEntry->sName = _pSceneHierarchy->GetResourceName( RESOURCE_TEXTURE, iTexture );
		const std::string sTextureFilename = _pSceneHierarchy->GetResourceFilename( RESOURCE_TEXTURE, pNewPoolEntry->sName );

		TTextureLoadData textureLoadData;

		const auto fileExtension = sTextureFilename.substr( sTextureFilename.rfind( '.' ) + 1 );
		assert( fileExtension.size() == 3 );

		if( fileExtension.compare( "png" ) == 0 )
		{
			if( !LoadTextureFromPNG( sTextureFilename, textureLoadData ) )
				printf( "!!!  TEXTURE LOAD FAILED: %s  !!!\n", sTextureFilename.c_str() );
		}
		else
		{
			if( !LoadTextureFromTarga( sTextureFilename, textureLoadData ) )
				printf( "!!!  TEXTURE LOAD FAILED: %s  !!!\n", sTextureFilename.c_str() );
		}

		// Setup the description of the texture.
		textureDesc.Height = textureLoadData.height;
		textureDesc.Width = textureLoadData.width;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		//Load new textures from file
		HRESULT hTextureResult = _pDevice->CreateTexture2D( &textureDesc, NULL, &pNewPoolEntry->pTexture );
		if( hTextureResult == S_OK )
			printf( "    = Texture successfully loaded from %s\n", sTextureFilename.c_str() );
		else
			printf( "!!!  TEXTURE LOAD FAILED: %s  !!!\n", sTextureFilename.c_str() );

		// Copy the targa image data into the texture.
		const auto rowPitch = ( textureLoadData.width * 4 ) * sizeof( unsigned char );
		_pDeviceContext->UpdateSubresource( pNewPoolEntry->pTexture, 0, NULL, textureLoadData.buffer, rowPitch, 0);

		// Load resource view
		resourceViewDesc.Format = textureDesc.Format;
		resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MostDetailedMip = 0;
		resourceViewDesc.Texture2D.MipLevels = -1;

		hTextureResult = _pDevice->CreateShaderResourceView( pNewPoolEntry->pTexture, &resourceViewDesc, &pNewPoolEntry->pResourceView );
		if( hTextureResult != S_OK )
			printf( "!!!  RESOURCE VIEW LOAD FAILED: %s  !!!\n", sTextureFilename.c_str() );

		_pDeviceContext->GenerateMips( pNewPoolEntry->pResourceView );

		SAFEDELETEARRAY( textureLoadData.buffer );
		
		m_TexturePool.push_back( pNewPoolEntry );
	}

	//Pass prefab types to entity manager
	for( unsigned int iPrefab = 0; iPrefab < _pSceneHierarchy->GetPrefabCount(); ++iPrefab )
	{
		TPrefabDefinition* pPrefab = _pSceneHierarchy->GetPrefabDefinition( iPrefab );
		if( pPrefab )
			AddPrefabToEntityManager( _pEntityManager, pPrefab );
	}
}

void
ResourceManager::AddPrefabToEntityManager( EntityManager* _pEntityManager, TPrefabDefinition* _pPrefab )
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

ID3D11ShaderResourceView*
ResourceManager::CreateTextureFromData( ID3D11Device* _pDevice, unsigned char* _pcData, std::string& _sTextureString, int _iWidth, int _iHeight )
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
	hResult = _pDevice->CreateShaderResourceView( pNewTexture, NULL, &pNewPoolData->pResourceView );
	HRCheck( hResult, L"Could not bind custom texture to shader resource view" );
	pNewPoolData->sName = _sTextureString;
	m_TexturePool.push_back( pNewPoolData );
	pNewTexture->Release();

	return pNewPoolData->pResourceView;
}

Model*
ResourceManager::GetModel( std::string& _pModelName ) const
{
	return ( m_mapModels.find( _pModelName )->second );
}

//CAnimatedModel*
//ResourceManager::GetAnimatedModel( std::string& _pcAnimatedModelName ) const
//{
//	std::string sKey = _pcAnimatedModelName;
//	return ( m_mapAnimations.find( sKey )->second );
//}

ID3D11ShaderResourceView*
ResourceManager::GetTexture( std::string& _pcTextureName ) const
{
	//Loop through texture vector and return texture matching this name
	ID3D11ShaderResourceView* pTexture = 0;
	for( unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture )
	{
		if( strcmp( m_TexturePool[ iTexture ]->sName.c_str(), _pcTextureName.c_str() ) == 0 )
		{
			pTexture = m_TexturePool[ iTexture ]->pResourceView;
			break;
		}
	}

	if( pTexture == 0 )
		Error( L"Could not find texture in pool!!" );
	
	return ( pTexture );
}

int
ResourceManager::GetTextureID( std::string& _pcTextureName ) const
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

int
ResourceManager::GetTextureID( ID3D11ShaderResourceView* _pTexture ) const
{
	//Loop through texture vector and return texture matching this name
	int iTextureID = -1;
	for( unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture )
	{
		if( m_TexturePool[ iTexture ]->pResourceView == _pTexture )
		{
			iTextureID = iTexture;
			break;
		}
	}

	if( iTextureID == -1 )
		Error( L"Could not find texture in pool!!" );
	
	return ( iTextureID );
}

void
ResourceManager::SendTextureDataToShader( ID3D11DeviceContext* _pDevContext )
{
	int iTextureCount = m_TexturePool.size();
	m_pTextureArray = new ID3D11ShaderResourceView*[ iTextureCount ];
	for( int iTexture = 0; iTexture < iTextureCount; ++iTexture )
	{
		m_pTextureArray[ iTextureCount - 1 - iTexture ] = m_TexturePool[ iTextureCount - iTexture - 1 ]->pResourceView;
	}
	_pDevContext->PSSetShaderResources( 0, iTextureCount, m_pTextureArray );

	delete[] m_pTextureArray;
	m_pTextureArray = 0;
}

bool
ResourceManager::LoadTextureFromTarga( const std::string& _textureName, TTextureLoadData& out_data )
{
	struct TargaHeader
	{
		unsigned char data1[ 12 ];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	TargaHeader targaFileHeader;
	FILE* filePtr;
	int error = 0;
	unsigned int count;
	unsigned char* targaImage;

	error = fopen_s( &filePtr, _textureName.c_str(), "rb" );
	if( error != 0 )
		return false;

	count = (unsigned int)fread( &targaFileHeader, sizeof( TargaHeader ), 1, filePtr );
	if( count != 1 )
		return false;

	// Get the important information from the header.
	out_data.height = (int)targaFileHeader.height;
	out_data.width = (int)targaFileHeader.width;

	if( targaFileHeader.bpp != 32 )
		return false;

	const auto imageSize = out_data.width * out_data.height * 4;
	targaImage = new unsigned char[ imageSize ];
	if( !targaImage )
	{
		return false;
	}

	// Read in the targa image data.
	count = (unsigned int)fread( targaImage, 1, imageSize, filePtr );
	if( count != imageSize )
	{
		return false;
	}

	// Close the file.
	error = fclose( filePtr );
	if( error != 0 )
	{
		return false;
	}

	// Allocate memory for the targa destination data.
	out_data.buffer = new unsigned char[ imageSize ];

	int index = 0;
	int k = ( out_data.width * out_data.height * 4 ) - ( out_data.width * 4 );
	for( unsigned j = 0; j < out_data.height; j++ )
	{
		for( unsigned i = 0; i < out_data.width; i++ )
		{
			out_data.buffer[ index + 0 ] = targaImage[ k + 2 ];  // Red.
			out_data.buffer[ index + 1 ] = targaImage[ k + 1 ];  // Green.
			out_data.buffer[ index + 2 ] = targaImage[ k + 0 ];  // Blue
			out_data.buffer[ index + 3 ] = targaImage[ k + 3 ];  // Alpha

			// Increment the indexes into the targa data.
			k += 4;
			index += 4;
		}

		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= ( out_data.width * 8 );
	}

	// Release the targa image data now that it was copied into the destination array.
	delete[] targaImage;
	targaImage = 0;

	return true;
}

bool 
ResourceManager::LoadTextureFromPNG( const std::string& _textureName, TTextureLoadData& out_data )
{
	std::vector<unsigned char> image; 
	unsigned width;
	unsigned height;

	unsigned error = lodepng::decode( image, width, height, _textureName.c_str() );
	if(error)
	{
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return false;
	}

	out_data.width = width;
	out_data.height = height;
	
	assert( image.size() > 0 );
	out_data.buffer = new unsigned char[ image.size() ];
	for( unsigned index = 0; index < image.size(); ++index )
		out_data.buffer[ index ] = image[ index ];

	return true;
}
