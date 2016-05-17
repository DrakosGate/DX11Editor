
#pragma once

// Library Includes
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Local Includes
#include "defines.h"
#include "irenderer.h"

enum EBlendTypes
{
	BLEND_INVALID = -1,
	BLEND_SOLID,
	BLEND_TRANSPARENT,
	BLEND_ALPHATOCOVERAGE,
	BLEND_MAX
};

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

class Clock;

//
class DX11Renderer : public IRenderer
{
	//Member functions
public:
	DX11Renderer();
	virtual ~DX11Renderer();

	virtual bool Initialise( HWND _hWnd, TSetupStruct* _pSetupData, int _iWindowWidth, int _iWindowHeight ) override;
	virtual void SetupDirectX11( HWND _hWnd, int _iWindowWidth, int _iWindowHeight );
	virtual void CleanUp();

	virtual void ExecuteOneFrame(Clock* _pClock, float _fDeltaTick);
	//virtual void PrepareLastScene();
	//
	//ID3D11RenderTargetView* GetRenderTargetView();
	//ID3D11DepthStencilView* GetDepthStencilView();
	//ID3D11ShaderResourceView* GetDepthSRV();
	//
	void SetBlendState(EBlendTypes _eBlendType);

private:
	DX11Renderer(const DX11Renderer& _krInstanceToCopy);
	const DX11Renderer& operator =(const DX11Renderer& _krInstanceToCopy);

//Member variables
private:
	//DX11
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	float m_pClearColour[4];
	TSetupStruct* m_pSetupData;
	HWND m_hWindow;

	//Depth stencil
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11ShaderResourceView* m_pDepthShaderResource;
	ID3D11BlendState** m_pBlendStates;
	ID3D11RasterizerState* m_pRasterState;

	//Device Information
	char m_videoCardDesc[ 128 ];
	int m_iVideoCardMemory;
	bool m_bIsFullscreen;
	bool m_bVSyncEnabled;

};

