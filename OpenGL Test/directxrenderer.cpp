
//
//  File Name   :   CDirectXRenderer.cpp
//  Description :   Code for Class CDirectXRenderer
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <iostream>
#include <math.h>
#include <D3D11.h>
#include <DXGI.h>
#include <D3Dcommon.h>

// Local Includes
#include "shader.h"
#include "camera.h"
#include "resourcemanager.h"
#include "level.h"
#include "clock.h"

// This Include
#include "directxrenderer.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CDirectXRenderer Constructor
*
* @author Christopher Howlett
*
*/
CDirectXRenderer::CDirectXRenderer()
: m_pDevice(0)
, m_pDeviceContext(0)
, m_pSwapChain(0)
, m_pRenderTargetView(0)
, m_pDepthStencilBuffer(0)
, m_pDepthStencilState(0)
, m_pDepthStencilView(0)
, m_iVideoCardMemory(0)
, m_bVSyncEnabled(false)
, m_bIsFullscreen(false)
, m_pLevel(0)
{
	//D3DXMatrixIdentity(&m_matWorld);
	//D3DXMatrixIdentity(&m_matProjection);
}
/**
*
* CDirectXRenderer Destructor
*
* @author Christopher Howlett
*
*/
CDirectXRenderer::~CDirectXRenderer()
{
	if (m_pLevel)
	{
		delete m_pLevel;
		m_pLevel = 0;
	}
	
	//Shutdown DX11
	CleanUp();
}
/**
*
* CDirectXRenderer Initialisation
*
* @author Christopher Howlett
* @param _hWnd Handle to window
* @param _iWindowWidth Window width
* @param _iWindowHeight Window Height
* @param _pInput Struct used for user input
*
*/
bool 
CDirectXRenderer::Initialise(HWND _hWnd, int _iWindowWidth, int _iWindowHeight, TInputStruct* _pInput)
{
	printf("Initialising DirectX11\n");
	m_hWindow = _hWnd;
	m_iWindowHeight = _iWindowHeight;
	m_iWindowWidth = _iWindowWidth;

	SetupDirectX11(_hWnd);
	
	float fAspectRatio = static_cast<float>(_iWindowWidth) / static_cast<float>(_iWindowHeight);
	
	m_pLevel = new CLevel();
	m_pLevel->Initialise(m_pDevice, m_pDeviceContext, this, _hWnd, _pInput, _iWindowWidth, _iWindowHeight);
	
	m_pClearColour[0] = 0.01f;
	m_pClearColour[1] = 0.01f;
	m_pClearColour[2] = 0.1f;
	m_pClearColour[3] = 1.0f;

	return true;
}
void 
CDirectXRenderer::ExecuteOneFrame(CClock* _pClock, float _fDeltaTick)
{
	m_pLevel->Process(m_pDevice, _pClock, _fDeltaTick);
	
	m_pLevel->Draw(m_pDeviceContext);
	m_pSwapChain->Present(m_bVSyncEnabled, 0);
}
void
CDirectXRenderer::PrepareLastScene()
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_pClearColour);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}
ID3D11RenderTargetView*
CDirectXRenderer::GetRenderTargetView()
{
	return m_pRenderTargetView;
}
ID3D11DepthStencilView*
CDirectXRenderer::GetDepthStencilView()
{
	return m_pDepthStencilView;
}
ID3D11ShaderResourceView*
CDirectXRenderer::GetDepthSRV()
{
	return m_pDepthShaderResource;
}
void 
CDirectXRenderer::SetBlendState(EBlendTypes _eBlendType)
{
	float fBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDeviceContext->OMSetBlendState(m_pBlendStates[_eBlendType], fBlendFactor, 0xffffffff);
}
/**
*
* CDirectXRenderer SetupDirectX11
*
* @author Christopher Howlett
* @param _hWnd Handle to window used for OpenGl
*
*/
void 
CDirectXRenderer::SetupDirectX11(HWND _hWnd)
{
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;
	IDXGIOutput* pAdapterOutput;
	DXGI_MODE_DESC* pDisplayModeList;
	DXGI_ADAPTER_DESC tAdapterDesc;
	DXGI_SWAP_CHAIN_DESC tSwapChainDesc;
	D3D_FEATURE_LEVEL tFeatureLevel;
	ID3D11Texture2D* pBackBuffer;

	D3D11_TEXTURE2D_DESC tDepthTextureDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC tStencilViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC tResourceDesc;
	ZeroMemory(&tResourceDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	D3D11_VIEWPORT tViewport;

	int iError;
	unsigned int iNumModes;
	unsigned int iStringLength;

	m_bVSyncEnabled = false;
	DXGI_FORMAT eFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT eDepthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//Setup factory and retrieve device settings
	HRCheck( CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory),											L"Factory Creation Failed");
	HRCheck( pFactory->EnumAdapters(0, &pAdapter),																	L"Adapter Retrieval Failed");
	HRCheck( pAdapter->EnumOutputs(0, &pAdapterOutput),																L"Adapter Output Retrieval Failed");
	HRCheck( pAdapterOutput->GetDisplayModeList(eFormat, DXGI_ENUM_MODES_INTERLACED, &iNumModes, NULL),				L"Could not retrieve display mode list");
	pDisplayModeList = new DXGI_MODE_DESC[iNumModes];
	HRCheck(pAdapterOutput->GetDisplayModeList(eFormat, DXGI_ENUM_MODES_INTERLACED, &iNumModes, pDisplayModeList),	L"Could not retrieve display mode list");

	float fFieldOfView = static_cast<float>(D3DX_PI) * 0.25f;
	float fScreenAspect = static_cast<float>(m_iWindowWidth) / static_cast<float>(m_iWindowHeight);
	unsigned int iNumerator = 1;
	unsigned int iDenominator = 1;
	//Find matching screen width and height
	for (unsigned int iMode = 0; iMode < iNumModes; ++iMode)
	{
		if (pDisplayModeList[iMode].Width == m_iWindowWidth && pDisplayModeList[iMode].Height == m_iWindowHeight)
		{
			iNumerator = pDisplayModeList[iMode].RefreshRate.Numerator;
			iDenominator = pDisplayModeList[iMode].RefreshRate.Denominator;
			break;
		}
	}
	//Get current adapters description
	HRCheck(pAdapter->GetDesc(&tAdapterDesc), L"Could not retrieve adapter description");
	m_iVideoCardMemory = static_cast<int>(tAdapterDesc.DedicatedVideoMemory / 1024 / 1024);
	iError = wcstombs_s(&iStringLength, m_videoCardDesc, 128, tAdapterDesc.Description, 128);

	//Clean up
	delete[] pDisplayModeList;
	pAdapterOutput->Release();
	pAdapter->Release();
	pFactory->Release();
	pDisplayModeList = 0;
	pAdapterOutput = 0;
	pAdapter = 0;
	pFactory = 0;

	ZeroMemory(&tSwapChainDesc, sizeof(tSwapChainDesc));
	tSwapChainDesc.BufferCount = 1;
	tSwapChainDesc.BufferDesc.Width = m_iWindowWidth;
	tSwapChainDesc.BufferDesc.Height = m_iWindowHeight;
	tSwapChainDesc.BufferDesc.Format = eFormat;
	if (m_bVSyncEnabled)
	{
		tSwapChainDesc.BufferDesc.RefreshRate.Numerator = iNumerator;
		tSwapChainDesc.BufferDesc.RefreshRate.Denominator = iDenominator;
	}
	else
	{
		tSwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		tSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	tSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tSwapChainDesc.OutputWindow = m_hWindow;

	//MULTISAMPLING
	tSwapChainDesc.SampleDesc.Count = 1;
	tSwapChainDesc.SampleDesc.Quality = 0;

	//FULLSCREEN
	tSwapChainDesc.Windowed = !m_bIsFullscreen;
	
	tSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 
	tSwapChainDesc.Flags = 0;

	tFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	//Add DEBUG flag only in debug mode
	unsigned int iCreationFlags = 0;
#if defined(_DEBUG)
	iCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//=================  CREATE DX11 DEVICE ====================
	HRCheck( D3D11CreateDeviceAndSwapChain(	NULL,
											D3D_DRIVER_TYPE_HARDWARE,
											NULL,
											iCreationFlags,
											&tFeatureLevel,
											1,
											D3D11_SDK_VERSION,
											&tSwapChainDesc,
											&m_pSwapChain,
											&m_pDevice,
											NULL,
											&m_pDeviceContext),
											L"Could not create DX11 Device");
	HRCheck(	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
				L"Could not retrieve back buffer pointer");

	HRCheck(	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView),
				L"Could not create Render Target View");
	pBackBuffer->Release();
	pBackBuffer = 0;

	// Create the depth/stencil buffer and view.
	tDepthTextureDesc.Width = WINDOW_WIDTH;
	tDepthTextureDesc.Height = WINDOW_HEIGHT;
	tDepthTextureDesc.MipLevels = 1;
	tDepthTextureDesc.ArraySize = 1;
	tDepthTextureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tDepthTextureDesc.SampleDesc.Count = 1;
	tDepthTextureDesc.SampleDesc.Quality = 0;
	tDepthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	tDepthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tDepthTextureDesc.CPUAccessFlags = 0;
	tDepthTextureDesc.MiscFlags = 0;

	HRCheck(m_pDevice->CreateTexture2D(&tDepthTextureDesc, 0, &m_pDepthStencilBuffer),
		L"Could not create depth stencil buffer texture");

	ZeroMemory(&tStencilViewDesc, sizeof(tStencilViewDesc));
	tStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	tStencilViewDesc.Texture2D.MipSlice = 0;

	HRCheck(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &tStencilViewDesc, &m_pDepthStencilView),
		L"Could not create depth stencil view");

	//Create depth stencil resource view
	//Create textures
	tResourceDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	tResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tResourceDesc.Texture2D.MostDetailedMip = 0;
	tResourceDesc.Texture2D.MipLevels = 1;
	HRCheck(m_pDevice->CreateShaderResourceView(m_pDepthStencilBuffer,
		&tResourceDesc,
		&m_pDepthShaderResource),
		L"Could not create Shader Resource View for depth stencil buffer");
	//=================  CREATE DEPTH STENCIL BUFFER ====================
	//ZeroMemory(&tDepthBufferDesc, sizeof(tDepthBufferDesc));
	//tDepthBufferDesc.Width = m_iWindowWidth;
	//tDepthBufferDesc.Height = m_iWindowHeight;
	//tDepthBufferDesc.MipLevels = 1;
	//tDepthBufferDesc.ArraySize = 1;
	//tDepthBufferDesc.Format = eDepthBufferFormat;
	//tDepthBufferDesc.SampleDesc.Count = 1;
	//tDepthBufferDesc.SampleDesc.Quality = 0;
	//tDepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//tDepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//tDepthBufferDesc.CPUAccessFlags = 0;
	//tDepthBufferDesc.MiscFlags = 0;
	//HRCheck(	m_pDevice->CreateTexture2D(&tDepthBufferDesc, NULL, &m_pDepthStencilBuffer),	
	//			L"Could not create depth stencil buffer");
	//
	////Create depth stencil state
	//ZeroMemory(&tDepthStencilDesc, sizeof(tDepthStencilDesc));
	//tDepthStencilDesc.DepthEnable = true;
	//tDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//tDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	//tDepthStencilDesc.StencilEnable = true;
	//tDepthStencilDesc.StencilReadMask = 0xFF;
	//tDepthStencilDesc.StencilWriteMask = 0xFF;
	////Frontface
	//tDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//tDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//tDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//tDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	////Backface
	//tDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//tDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//tDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//tDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//HRCheck(	m_pDevice->CreateDepthStencilState(&tDepthStencilDesc, &m_pDepthStencilState),
	//			L"Could not create depth stencil state");
	//m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	//
	////Create depth stencil view
	//ZeroMemory(&tStencilViewDesc, sizeof(tStencilViewDesc));
	//tStencilViewDesc.Format = eDepthBufferFormat;
	//tStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//tStencilViewDesc.Texture2D.MipSlice = 0;
	//HRCheck(	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &tStencilViewDesc, &m_pDepthStencilView),
	//			L"Could not create depth stencil view");
	//
	////Bind render target view and deoth stencil buffer
	//m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//Setup Viewport
	tViewport.Width = static_cast<float>(m_iWindowWidth);
	tViewport.Height = static_cast<float>(m_iWindowHeight);
	tViewport.MinDepth = 0.0f;
	tViewport.MaxDepth = 1.0f;
	tViewport.TopLeftX = 0.0f;
	tViewport.TopLeftY = 0.0f;
	m_pDeviceContext->RSSetViewports(1, &tViewport);

	//D3DXMatrixPerspectiveFovLH(&m_matProjection, fFieldOfView, fScreenAspect, 0.1f, 1000.0f);
	//D3DXMatrixIdentity(&m_matWorld);
	m_pBlendStates = new ID3D11BlendState*[BLEND_MAX];

	D3D11_BLEND_DESC tBlendState;
	ZeroMemory(&tBlendState, sizeof(D3D11_BLEND_DESC));
	tBlendState.RenderTarget[0].BlendEnable = FALSE;
	tBlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tBlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	tBlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tBlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	tBlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	tBlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tBlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	tBlendState.AlphaToCoverageEnable = FALSE;
	
	m_pDevice->CreateBlendState(&tBlendState, &m_pBlendStates[BLEND_SOLID]);
	tBlendState.RenderTarget[0].BlendEnable = TRUE;
	m_pDevice->CreateBlendState(&tBlendState, &m_pBlendStates[BLEND_TRANSPARENT]);
	tBlendState.AlphaToCoverageEnable = TRUE;
	m_pDevice->CreateBlendState(&tBlendState, &m_pBlendStates[BLEND_ALPHATOCOVERAGE]);

	SetBlendState(BLEND_TRANSPARENT);
}
void 
CDirectXRenderer::CleanUp()
{
	if (m_pSwapChain)
	{
		m_pSwapChain->SetFullscreenState(false, NULL);
	}

	//Release DX11 stuff
	for (int iBlendState = 0; iBlendState < BLEND_MAX; ++iBlendState)
	{
		ReleaseCOM(m_pBlendStates[iBlendState]);
	}
	SAFEDELETEARRAY(m_pBlendStates);

	ReleaseCOM(m_pDepthShaderResource);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pDepthStencilState);
	ReleaseCOM(m_pDepthStencilBuffer);
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDeviceContext);
	ReleaseCOM(m_pDevice);
	ReleaseCOM(m_pSwapChain);
}