
// Library Includes
#include <iostream>
#include <math.h>
#include <D3D11.h>
#include <DXGI.h>
#include <D3Dcommon.h>

// Local Includes
#include "clock.h"
#include "datastructures.h"

// This Include
#include "dx11renderer.h"

// Static Variables

// Static Function Prototypes

// Implementation

DX11Renderer::DX11Renderer()
	: m_pDevice( nullptr )
	, m_pDeviceContext( nullptr )
	, m_pSwapChain( nullptr )
	, m_pRenderTargetView( nullptr )
	, m_pSetupData( nullptr )
	, m_hWindow( NULL )
	, m_pDepthStencilBuffer( nullptr )
	, m_pDepthStencilState( nullptr )
	, m_pDepthStencilView( nullptr )
	, m_pDepthShaderResource( nullptr )
	, m_pBlendStates( nullptr )
	, m_pRasterState( nullptr )
	, m_iVideoCardMemory( 0 )
	, m_bIsFullscreen( false )
	, m_bVSyncEnabled( false )
{

}

DX11Renderer::~DX11Renderer()
{
	SAFEDELETE( m_pSetupData );

	//Shutdown DX11
	CleanUp();
}

bool
DX11Renderer::Initialise( HWND _hWnd, TSetupStruct* _pSetupData, int _iWindowWidth, int _iWindowHeight )
{
	m_pSetupData = _pSetupData;
	printf( "Initialising DirectX11\n" );

	SetupDirectX11( _hWnd, _iWindowWidth, _iWindowHeight );

	float fAspectRatio = static_cast<float>( _iWindowWidth ) / static_cast<float>( _iWindowHeight );

	m_pClearColour[ 0 ] = 0.01f;
	m_pClearColour[ 1 ] = 0.01f;
	m_pClearColour[ 2 ] = 0.1f;
	m_pClearColour[ 3 ] = 1.0f;

	return true;
}

void
DX11Renderer::ExecuteOneFrame( Clock* _pClock, float _fDeltaTick )
{
	m_pSwapChain->Present( m_bVSyncEnabled, 0 );
}

void
DX11Renderer::SetupDirectX11( HWND _hWnd, int _iWindowWidth, int _iWindowHeight )
{
	const auto displayFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;

	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;

	D3D11_VIEWPORT viewport;

	HR( CreateDXGIFactory( __uuidof( IDXGIFactory ), (void**)&factory ) );
	HR( factory->EnumAdapters( 0, &adapter ) );
	HR( adapter->EnumOutputs( 0, &adapterOutput ) );

	unsigned numModes;
	HR( adapterOutput->GetDisplayModeList( displayFormat, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL ) );
	displayModeList = new DXGI_MODE_DESC[ numModes ];
	assert( displayModeList );
	HR( adapterOutput->GetDisplayModeList( displayFormat, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList ) );
	unsigned numerator;
	unsigned denominator;
	for( unsigned i = 0; i < numModes; ++i )
	{
		if( displayModeList[ i ].Width == (unsigned int)_iWindowWidth )
		{
			if( displayModeList[ i ].Height == (unsigned int)_iWindowHeight )
			{
				numerator = displayModeList[ i ].RefreshRate.Numerator;
				denominator = displayModeList[ i ].RefreshRate.Denominator;
			}
		}
	}

	HR( adapter->GetDesc( &adapterDesc ) );
	m_iVideoCardMemory = (int)( adapterDesc.DedicatedVideoMemory / 1024 / 1024 );

	unsigned stringLength;
	wcstombs_s( &stringLength, m_videoCardDesc, 128, adapterDesc.Description, 128 );

	SAFEDELETEARRAY( displayModeList );

	adapterOutput->Release();
	adapter->Release();
	factory->Release();

	adapterOutput = 0;
	adapter = 0;
	factory = 0;

	// Initialise Swap Chain
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = _iWindowWidth;
	swapChainDesc.BufferDesc.Height = _iWindowHeight;
	swapChainDesc.BufferDesc.Format = displayFormat;
	if( m_bVSyncEnabled )
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = _hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !m_bIsFullscreen;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Create device with debug layer for Debug builds
	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// DEVICE AND SWAP CHAIN
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	HR( D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext ) );

	// Get back buffer texture
	HR( m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&backBufferPtr ) );

	// Create render target view using this
	HR( m_pDevice->CreateRenderTargetView( backBufferPtr, NULL, &m_pRenderTargetView ) );
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	// DEPTH BUFFER
	ZeroMemory( &depthBufferDesc, sizeof( depthBufferDesc ) );
	depthBufferDesc.Width = _iWindowWidth;
	depthBufferDesc.Height = _iWindowHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create depth buffer texture
	HR( m_pDevice->CreateTexture2D( &depthBufferDesc, NULL, &m_pDepthStencilBuffer ) );

	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR( m_pDevice->CreateDepthStencilState( &depthStencilDesc, &m_pDepthStencilState ) );
	m_pDeviceContext->OMSetDepthStencilState( m_pDepthStencilState, 1 );

	// DEPTH VIEW
	ZeroMemory( &depthStencilViewDesc, sizeof( depthStencilViewDesc ) );
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR( m_pDevice->CreateDepthStencilView( m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView ) );

	// Set Render targets 
	m_pDeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );

	// RASTER STATE
	ZeroMemory( &rasterDesc, sizeof( rasterDesc ) );
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// BLEND STATES
	m_pBlendStates = new ID3D11BlendState*[ BLEND_MAX ];

	D3D11_BLEND_DESC tBlendState;
	ZeroMemory( &tBlendState, sizeof( D3D11_BLEND_DESC ) );
	tBlendState.RenderTarget[ 0 ].BlendEnable = FALSE;
	tBlendState.RenderTarget[ 0 ].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tBlendState.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	tBlendState.RenderTarget[ 0 ].BlendOp = D3D11_BLEND_OP_ADD;
	tBlendState.RenderTarget[ 0 ].SrcBlendAlpha = D3D11_BLEND_ONE;
	tBlendState.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ZERO;
	tBlendState.RenderTarget[ 0 ].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tBlendState.RenderTarget[ 0 ].RenderTargetWriteMask = 0x0f;
	tBlendState.AlphaToCoverageEnable = FALSE;

	m_pDevice->CreateBlendState( &tBlendState, &m_pBlendStates[ BLEND_SOLID ] );
	tBlendState.RenderTarget[ 0 ].BlendEnable = TRUE;
	m_pDevice->CreateBlendState( &tBlendState, &m_pBlendStates[ BLEND_TRANSPARENT ] );
	tBlendState.AlphaToCoverageEnable = TRUE;
	m_pDevice->CreateBlendState( &tBlendState, &m_pBlendStates[ BLEND_ALPHATOCOVERAGE ] );

	SetBlendState( BLEND_TRANSPARENT );

	// Create the rasterizer state from the description we just filled out.
	HR( m_pDevice->CreateRasterizerState( &rasterDesc, &m_pRasterState ) );

	// Now set the rasterizer state.
	m_pDeviceContext->RSSetState( m_pRasterState );

	// VIEWPORT
	viewport.Width = (float)_iWindowWidth;
	viewport.Height = (float)_iWindowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_pDeviceContext->RSSetViewports( 1, &viewport );


}

void
DX11Renderer::CleanUp()
{
	if( m_pSwapChain )
	{
		m_pSwapChain->SetFullscreenState( false, NULL );
	}

	//Release DX11 stuff
	if( m_pBlendStates != nullptr )
	{
		for( int iBlendState = 0; iBlendState < BLEND_MAX; ++iBlendState )
		{
			ReleaseCOM( m_pBlendStates[ 0 ] );
		}
		SAFEDELETEARRAY( m_pBlendStates );
	}

	ReleaseCOM( m_pDepthShaderResource );
	ReleaseCOM( m_pDepthStencilView );
	ReleaseCOM( m_pDepthStencilState );
	ReleaseCOM( m_pDepthStencilBuffer );
	ReleaseCOM( m_pRenderTargetView );
	ReleaseCOM( m_pDeviceContext );
	ReleaseCOM( m_pDevice );
	ReleaseCOM( m_pSwapChain );
}

void
DX11Renderer::PrepareLastScene()
{
	m_pDeviceContext->ClearRenderTargetView( m_pRenderTargetView, m_pClearColour );
	m_pDeviceContext->ClearDepthStencilView( m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	m_pDeviceContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthStencilView );
}

ID3D11RenderTargetView*
DX11Renderer::GetRenderTargetView()
{
	return m_pRenderTargetView;
}

ID3D11DepthStencilView*
DX11Renderer::GetDepthStencilView()
{
	return m_pDepthStencilView;
}

ID3D11ShaderResourceView*
DX11Renderer::GetDepthSRV()
{
	return m_pDepthShaderResource;
}

void
DX11Renderer::SetBlendState( EBlendTypes _eBlendType )
{
	float fBlendFactor[ 4 ] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pDeviceContext->OMSetBlendState( m_pBlendStates[ _eBlendType ], fBlendFactor, 0xffffffff );
}
