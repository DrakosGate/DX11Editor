//
//  File Name   :   CDirectXRenderer.h
//  Description :   Class of DirectXRenderer
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __DIRECTXRENDERER_H__
#define __DIRECTXRENDERER_H__

// Library Includes

// Local Includes
#include "mathlibrary.h"
#include "irenderer.h"

// Types
enum EBlendTypes
{
	BLEND_INVALID = -1,
	BLEND_SOLID,
	BLEND_TRANSPARENT,
	BLEND_ALPHATOCOVERAGE,
	BLEND_MAX
};

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11InputLayout;
struct ID3D11BlendState;
struct IDXGISwapChain;
struct TSetupStruct;

class CClock;
class CLevel;
class CModel;
class CCamera;

class CDirectXRenderer : public IRenderer
{
	//Member functions
public:
	CDirectXRenderer();
	virtual ~CDirectXRenderer();

	virtual bool Initialise( HWND _hWnd, TSetupStruct* _pSetupData, int _iWindowWidth, int _iWindowHeight );
	virtual void SetupDirectX11( HWND _hWnd );
	virtual void CleanUp();

	virtual void ExecuteOneFrame( CClock* _pClock, float _fDeltaTick );
	virtual void PrepareLastScene();

	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();
	ID3D11ShaderResourceView* GetDepthSRV();

	void SetBlendState( EBlendTypes _eBlendType );

private:
	CDirectXRenderer( const CDirectXRenderer& _krInstanceToCopy );
	const CDirectXRenderer& operator =( const CDirectXRenderer& _krInstanceToCopy );

	//Member variables
private:
	//DX11
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	float m_pClearColour[ 4 ];
	TSetupStruct* m_pSetupData;

	//Device Information
	char m_videoCardDesc[ 128 ];
	int m_iVideoCardMemory;
	bool m_bIsFullscreen;
	bool m_bVSyncEnabled;

	//Depth stencil
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11ShaderResourceView* m_pDepthShaderResource;
	ID3D11BlendState** m_pBlendStates;

	CLevel* m_pLevel;
};

#endif //DIRECTXRENDERER