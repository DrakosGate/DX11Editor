
// Library Includes
#include <D3D11.h>
#include <d3dcompiler.h>
//#include <D3DX11.h>

// Local Includes
#include "defines.h"
#include "camera.h"

#include "lightmanager.h"
#include "pointlight.h"
#include "spotlight.h"
#include "directionallight.h"

// This Include
#include "shader.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* Shader class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
Shader::Shader()
	: m_pVertexShader( 0 )
	, m_pPixelShader( 0 )
	, m_pGeometryShader( 0 )
{

}

/**
*
* Shader class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
Shader::~Shader()
{
	ReleaseCOM( pVertexShaderBuffer );
	ReleaseCOM( pPixelShaderBuffer );
	ReleaseCOM( pGeometryShaderBuffer );

	ReleaseCOM( m_pVertexShader );
	ReleaseCOM( m_pPixelShader );
	ReleaseCOM( m_pGeometryShader );
	for( int iBuffer = 0; iBuffer < BUFFER_MAX; ++iBuffer )
	{
		ReleaseCOM( m_pShaderBuffers[ iBuffer ] );
	}
}
/**
*
* Shader class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 11 device
* @return Returns true
*
*/
bool
Shader::Initialise( ID3D11Device* _pDevice )
{
	bool bResult = true;

	D3D11_BUFFER_DESC matBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	ZeroMemory( &matBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
	ZeroMemory( &lightBufferDesc, sizeof( D3D11_BUFFER_DESC ) );

	//Create matrix buffer
	matBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matBufferDesc.ByteWidth = sizeof( TMatBuffer );
	matBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matBufferDesc.MiscFlags = 0;
	matBufferDesc.StructureByteStride = 0;
	HRCheck( _pDevice->CreateBuffer( &matBufferDesc, NULL, &m_pShaderBuffers[ BUFFER_MATRICES ] ),
		L"Failed to create Matrix CBUFFER" );

	const auto a = sizeof( Math::Vector3 );

	//Create buffer for lighting
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof( TLightBuffer );
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	HRCheck( _pDevice->CreateBuffer( &lightBufferDesc, NULL, &m_pShaderBuffers[ BUFFER_LIGHTING ] ),
		L"Failed to create Lighting CBUFFER" );

	return bResult;
}
void
Shader::CompileVertexShader( ID3D11Device* _pDevice, wchar_t* _pcVSFilename, char* _pcVSFunction )
{
	ID3D10Blob* pErrorMessage;
	unsigned int iShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(_DEBUG)
	iShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	if( _pcVSFilename && _pcVSFunction )
	{
		//Compile Vertex Shader
		HRESULT hVSResult = D3DCompileFromFile( _pcVSFilename,
			NULL,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_pcVSFunction,
			"vs_5_0",
			iShaderFlags,
			0,
			&pVertexShaderBuffer,
			&pErrorMessage );
		BlobCheck( hVSResult, pErrorMessage );
		//Populate buffers
		HRCheck( _pDevice->CreateVertexShader( pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader ),
			L"Could not create vertex shader from buffer" );
	}
}
void
Shader::CompilePixelShader( ID3D11Device* _pDevice, wchar_t* _pcPSFilename, char* _pcPSFunction )
{
	ID3D10Blob* pErrorMessage;
	unsigned int iShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(_DEBUG)
	iShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	if( _pcPSFilename && _pcPSFunction )
	{
		//Compile Pixel Shader
		HRESULT hPSResult = D3DCompileFromFile( _pcPSFilename,
			NULL,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_pcPSFunction,
			"ps_5_0",
			iShaderFlags,
			0,
			&pPixelShaderBuffer,
			&pErrorMessage );
		BlobCheck( hPSResult, pErrorMessage );

		//Populate buffers
		HRCheck( _pDevice->CreatePixelShader( pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader ),
			L"Could not create pixel shader from buffer" );
	}
}
void
Shader::CompileGeometryShader( ID3D11Device* _pDevice, wchar_t* _pcGSFilename, char* _pcGSFunction )
{
	ID3D10Blob* pErrorMessage;
	unsigned int iShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined(_DEBUG)
	iShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	if( _pcGSFilename && _pcGSFunction )
	{
		//Compile Pixel Shader
		HRESULT hPSResult = D3DCompileFromFile( _pcGSFilename,
			NULL,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_pcGSFunction,
			"gs_5_0",
			iShaderFlags,
			0,
			&pGeometryShaderBuffer,
			&pErrorMessage );
		BlobCheck( hPSResult, pErrorMessage );

		//Populate buffers
		HRCheck( _pDevice->CreateGeometryShader( pGeometryShaderBuffer->GetBufferPointer(), pGeometryShaderBuffer->GetBufferSize(), NULL, &m_pGeometryShader ),
			L"Could not create geometry shader from buffer" );
	}
}
void
Shader::SendWVPMatrixData( ID3D11DeviceContext* _pDeviceContext, Math::Matrix* _pWorld, Math::Matrix* _pView, Math::Matrix* _pProjection )
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	TMatBuffer* pMatrixBuffer;
	unsigned int uiBufferNumber;

	// Map Matrix Buffer
	HRCheck( _pDeviceContext->Map( m_pShaderBuffers[ BUFFER_MATRICES ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource ),
		L"Failed to map Matrix Buffer" );

	pMatrixBuffer = reinterpret_cast<TMatBuffer*>( mappedResource.pData );
	pMatrixBuffer->matWorld = Math::MatrixTranspose( *_pWorld ).GetMatrix();
	pMatrixBuffer->matView  = Math::MatrixTranspose( *_pView ).GetMatrix();
	pMatrixBuffer->matProj  = Math::MatrixTranspose( *_pProjection ).GetMatrix();

	//Unmap matrix buffer
	_pDeviceContext->Unmap( m_pShaderBuffers[ BUFFER_MATRICES ], 0 );

	uiBufferNumber = 0;
	//Set matrix buffer values
	_pDeviceContext->VSSetConstantBuffers( 0, 1, &m_pShaderBuffers[ BUFFER_MATRICES ] );
	if( m_pGeometryShader )
	{
		_pDeviceContext->GSSetConstantBuffers( 0, 1, &m_pShaderBuffers[ BUFFER_MATRICES ] );
	}
}
void
Shader::SendLightInformation( ID3D11DeviceContext* _pDeviceContext, LightManager* _pLightManager, Camera* _pActiveCamera )
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	TLightBuffer* pLightData;
	//Lock light buffer
	HRCheck( _pDeviceContext->Map( m_pShaderBuffers[ BUFFER_LIGHTING ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource ),
		L"Failed to map lighting buffer" );

	pLightData = reinterpret_cast<TLightBuffer*>( mappedResource.pData );
	//Copy point lights into buffer
	int iCurrentIndex = 0;
	int iNumLights = _pLightManager->GetLightCount( LIGHT_POINT );
	for( int iLight = 0; iLight < iNumLights; ++iLight )
	{
		pLightData->lightInfo[ iCurrentIndex ] = _pLightManager->GetPoint( iLight )->GetLightInfo();
		++iCurrentIndex;
	}
	//Copy spot lights into buffer
	iNumLights = _pLightManager->GetLightCount( LIGHT_SPOT );
	for( int iLight = 0; iLight < iNumLights; ++iLight )
	{
		pLightData->lightInfo[ iCurrentIndex ] = _pLightManager->GetSpot( iLight )->GetLightInfo();
		++iCurrentIndex;
	}
	//Copy directional lights into buffer
	iNumLights = _pLightManager->GetLightCount( LIGHT_DIRECTIONAL );
	for( int iLight = 0; iLight < iNumLights; ++iLight )
	{
		pLightData->lightInfo[ iCurrentIndex ] = _pLightManager->GetDirectional( iLight )->GetLightInfo();
		++iCurrentIndex;
	}

	pLightData->veCameraPos = _pActiveCamera->GetPosition();
	pLightData->iActiveLightCount = _pLightManager->GetLightCount();

	_pDeviceContext->Unmap( m_pShaderBuffers[ BUFFER_LIGHTING ], 0 );
	unsigned int iBufferNumber = 1;
	_pDeviceContext->PSSetConstantBuffers( 0, 1, &m_pShaderBuffers[ BUFFER_LIGHTING ] );
}
ID3D11VertexShader*
Shader::GetVertexShader() const
{
	return m_pVertexShader;
}
ID3D11GeometryShader*
Shader::GetGeometryShader() const
{
	return m_pGeometryShader;
}
ID3D11PixelShader*
Shader::GetPixelShader() const
{
	return m_pPixelShader;
}
ID3D10Blob*
Shader::GetShaderBlob() const
{
	return pVertexShaderBuffer;
}