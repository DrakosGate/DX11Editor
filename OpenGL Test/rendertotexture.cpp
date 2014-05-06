//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CRenderToTexture.cpp
//  Description :   Code for Class CRenderToTexture
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <D3D11.h>

// Local Includes
#include "defines.h"

// This Include
#include "rendertotexture.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CRenderToTexture class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CRenderToTexture::CRenderToTexture()
:	m_pRenderTexture(0)
,	m_pRenderTargetView(0)
,	m_pShaderResourceView(0)
,	m_bHasRenderTarget(false)
,	m_bHasDepthStencil(false)
,	m_pDepthStencilView(0)
,	m_pDepthStencilBuffer(0)
,	m_pAddDepthState(0)
,	m_pSubDepthState(0)
{

}
/**
*
* CRenderToTexture class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CRenderToTexture::~CRenderToTexture()
{
	if(m_bHasRenderTarget)
	{
		ReleaseCOM(m_pRenderTexture);
		ReleaseCOM(m_pRenderTargetView);
		ReleaseCOM(m_pShaderResourceView);
	}
	if(m_bHasDepthStencil)
	{
		ReleaseCOM(m_pDepthStencilView);
		ReleaseCOM(m_pDepthStencilBuffer);
		ReleaseCOM(m_pDepthResourceView);

		ReleaseCOM(m_pAddDepthState);
		ReleaseCOM(m_pSubDepthState);
	}
}
/**
*
* CRenderToTexture class initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice Directx 10 Device
* @param _bHasRenderTarget Specifies whether this has a colour texture
* @param _bHasDepthStencil Specifies whether this has a depth stencil 
* @param _iWidth Viewport width
* @param _iHeight Viewport height
* @return Returns success of initialisation
*
*/
bool 
CRenderToTexture::Initialise(ID3D11Device* _pDevice, bool _bHasRenderTarget, bool _bHasDepthStencil, int _iWidth, int _iHeight)
{
	m_bHasRenderTarget = _bHasRenderTarget;
	m_bHasDepthStencil = _bHasDepthStencil;

	//Create Render target
	if(m_bHasRenderTarget)
	{
		CreateRenderTarget(_pDevice, _iWidth, _iHeight);
	}
	if(m_bHasDepthStencil)
	{
		CreateDepthStencil(_pDevice, _iWidth, _iHeight);
	}
	
	return true;
}
/**
*
* CRenderToTexture class initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice Directx 10 Device
* @param _iWidth Viewport width
* @param _iHeight Viewport height
*
*/
void 
CRenderToTexture::CreateRenderTarget(ID3D11Device* _pDevice, int _iWidth, int _iHeight)
{
	// Zero memory of all structures needed
	D3D11_TEXTURE2D_DESC tTextureDesc;
	ZeroMemory(&tTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	D3D11_RENDER_TARGET_VIEW_DESC tRenderTargetDesc;
	ZeroMemory(&tRenderTargetDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	D3D11_SHADER_RESOURCE_VIEW_DESC tShaderResourceDesc;
    ZeroMemory(&tShaderResourceDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	
	//Create texture description
	tTextureDesc.Width = _iWidth;
	tTextureDesc.Height = _iHeight;
	tTextureDesc.MipLevels = 1;
	tTextureDesc.ArraySize = 1;
	tTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	tTextureDesc.SampleDesc.Count = 1;
	tTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	tTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tTextureDesc.CPUAccessFlags = 0;
	tTextureDesc.MiscFlags = 0;

	HRCheck(_pDevice->CreateTexture2D(&tTextureDesc, NULL, &m_pRenderTexture),
			L"Could not create render target texture");

	//Create RenderTargetView description
	tRenderTargetDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tRenderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	tRenderTargetDesc.Texture2D.MipSlice = 0;

	HRCheck(_pDevice->CreateRenderTargetView(m_pRenderTexture, &tRenderTargetDesc, &m_pRenderTargetView),
			L"Could not create Render target view"); 
	
	//Create shader resource description
	tShaderResourceDesc.Format = tRenderTargetDesc.Format;
    tShaderResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    tShaderResourceDesc.Texture2D.MostDetailedMip = 0;
    tShaderResourceDesc.Texture2D.MipLevels = 1;

	HRCheck(_pDevice->CreateShaderResourceView(	m_pRenderTexture, &tShaderResourceDesc,	&m_pShaderResourceView),
			L"Could not create shader resource view");
}
/**
*
* CRenderToTexture class initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice Directx 10 Device
* @param _iWidth Viewport width
* @param _iHeight Viewport height
*
*/
void 
CRenderToTexture::CreateDepthStencil(ID3D11Device* _pDevice, int _iWidth, int _iHeight)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_TEXTURE2D_DESC));
	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	D3D11_DEPTH_STENCIL_DESC tDepthStencilDesc;
	ZeroMemory(&tDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	D3D11_SHADER_RESOURCE_VIEW_DESC tResourceDesc;
    ZeroMemory(&tResourceDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
   
	// Create the depth/stencil buffer and view.
	depthStencilDesc.Width     = _iWidth;
	depthStencilDesc.Height    = _iHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count   = 1; 
	depthStencilDesc.SampleDesc.Quality = 0; 
	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	HRCheck(_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer),
			L"Could not create depth stencil buffer texture");

	 ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthDesc.Texture2D.MipSlice = 0;
       
	HRCheck(_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthDesc, &m_pDepthStencilView),
			L"Could not create depth stencil view");
	
	//Setup depth stencil state
	ZeroMemory(&tDepthStencilDesc, sizeof(tDepthStencilDesc));
	tDepthStencilDesc.DepthEnable = true;
	tDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	tDepthStencilDesc.StencilEnable = true;
	tDepthStencilDesc.StencilReadMask = 0xff;
	tDepthStencilDesc.StencilWriteMask = 0xff;
	
	tDepthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	tDepthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	tDepthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	tDepthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	tDepthStencilDesc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	tDepthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	tDepthStencilDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	tDepthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	HRCheck(_pDevice->CreateDepthStencilState(&tDepthStencilDesc, &m_pAddDepthState),
			L"Could not create additive depth stencil state");
	//_pDevice->OMSetDepthStencilState(m_pAddDepthState, 0);

	//Setup depth stencil state
	tDepthStencilDesc.DepthEnable = true;
	tDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
	tDepthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	tDepthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	tDepthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;
	tDepthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_EQUAL;

	HRCheck(_pDevice->CreateDepthStencilState(&tDepthStencilDesc, &m_pSubDepthState),
			L"Could not create subtractive depth stencil state");
	

	//Create depth stencil resource view
	//Create textures
	tResourceDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    tResourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    tResourceDesc.Texture2D.MostDetailedMip = 0;
    tResourceDesc.Texture2D.MipLevels = 1;
	HRCheck(_pDevice->CreateShaderResourceView(	m_pDepthStencilBuffer,
												&tResourceDesc,
												&m_pDepthResourceView),
			L"Could not create Shader Resource View for depth stencil buffer");
}
/**
*
* CRenderToTexture class SetRenderTarget
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice Directx 10 Device
* @param _pDepthStencilView Depth stencil view
*
*/
void 
CRenderToTexture::SetRenderTarget(ID3D11DeviceContext* _pDevice, int _iIndex, ID3D11DepthStencilView* _pDepthStencilView)
{
	if(m_bHasDepthStencil)
	{
		_pDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	}
	else
	{
		_pDevice->OMSetRenderTargets(1, &m_pRenderTargetView, _pDepthStencilView);
	}
}
/**
*
* CRenderToTexture class ClearRenderTarget
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice Directx 10 Device
* @param _pDepthStencilView Depth stencil view
* @param _pClearColour Colour to clear
*
*/
void 
CRenderToTexture::ClearRenderTarget(ID3D11DeviceContext* _pDevice, ID3D11DepthStencilView* _pDepthStencilView, float* _pClearColour)
{
	if(m_bHasRenderTarget)
	{
		_pDevice->ClearRenderTargetView(m_pRenderTargetView, _pClearColour);
	}
	if(m_bHasDepthStencil)
	{
		_pDevice->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	else
	{
		if(_pDepthStencilView)
		{
			_pDevice->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}
}
/**
*
* CRenderToTexture class GetShaderResourceView
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns Shader resource view
*
*/
ID3D11ShaderResourceView* 
CRenderToTexture::GetRenderShaderResource()
{
	return m_pShaderResourceView;
}
/**
*
* CRenderToTexture class GetDepthShaderResource
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns Depth shader resource view
*
*/
ID3D11ShaderResourceView* 
CRenderToTexture::GetDepthShaderResource()
{
	return m_pDepthResourceView;
}
ID3D11DepthStencilView* 
CRenderToTexture::GetDepthStencilView()
{
	return m_pDepthStencilView;
}
/**
*
* CRenderToTexture class GetStencilBuffer
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns stencil buffer
*
*/
ID3D11Texture2D*  
CRenderToTexture::GetDepthStencilTexture()
{
	return m_pDepthStencilBuffer;
}
/**
*
* CRenderToTexture class GetRenderTarget
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns pointer to render target view
*
*/
ID3D11RenderTargetView*  
CRenderToTexture::GetRenderTarget()
{
	return m_pRenderTargetView;
}
/**
*
* CRenderToTexture class GetAddDepthState
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns add stencil state
*
*/
ID3D11DepthStencilState* 
CRenderToTexture::GetAddDepthState()
{
	return m_pAddDepthState;
}
/**
*
* CRenderToTexture class GetSubDepthState
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns add stencil state
*
*/
ID3D11DepthStencilState* 
CRenderToTexture::GetSubDepthState()
{
	return m_pSubDepthState;
}