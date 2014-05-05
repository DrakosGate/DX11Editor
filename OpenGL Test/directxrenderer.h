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
#include <D3DX10Math.h>

// Local Includes
#include "chrismaths.h"
#include "irenderer.h"

// Types

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

class CLevel;
class CModel;
class CCamera;

class CDirectXRenderer : public IRenderer
{
	//Member functions
public:
	CDirectXRenderer();
	virtual ~CDirectXRenderer();

	virtual bool Initialise(HWND _hWnd, int _iWindowWidth, int _iWindowHeight, TInputStruct* _pInput);
	virtual void SetupDirectX11(HWND _hWnd);
	virtual void CleanUp();

	virtual void ExecuteOneFrame(float _fDeltaTick);
	virtual void PrepareLastScene();

private:
	CDirectXRenderer(const CDirectXRenderer& _krInstanceToCopy);
	const CDirectXRenderer& operator =(const CDirectXRenderer& _krInstanceToCopy);

//Member variables
private:
	//DX11
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	float m_pClearColour[4];

	//Device Information
	char m_videoCardDesc[128];
	int m_iVideoCardMemory;
	bool m_bIsFullscreen;
	bool m_bVSyncEnabled;

	//Depth stencil
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11BlendState* m_pTransparentBlendState;

	CLevel* m_pLevel;
};

#endif //DIRECTXRENDERER