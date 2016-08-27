//
//  File Name   :   RenderToTexture.h
//  Description :   Class of RenderToTexture
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __RenderToTexture_H__
#define __RenderToTexture_H__

// Library Includes

// Local Includes

// Types

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilState;


class RenderToTexture
{
public:
	RenderToTexture();
	~RenderToTexture();

	bool Initialise(ID3D11Device* _pDevice, bool _bHasRenderTarget, bool _bHasDepthStencil, int _iWidth, int _iHeight);
	void Draw();
	void CreateRenderTarget(ID3D11Device* _pDevice, int _iWidth, int _iHeight);
	void CreateDepthStencil(ID3D11Device* _pDevice, int _iWidth, int _iHeight);

	void SetRenderTarget(ID3D11DeviceContext* _pDevice, int _iIndex, ID3D11DepthStencilView* _pDepthStencilView);
	void ClearRenderTarget(ID3D11DeviceContext* _pDevice, ID3D11DepthStencilView* _pDepthStencilView, float* _pClearColour);
	ID3D11ShaderResourceView* GetRenderShaderResource();
	ID3D11ShaderResourceView* GetDepthShaderResource();
	ID3D11DepthStencilView* GetDepthStencilView();
	ID3D11Texture2D* GetDepthStencilTexture();
	ID3D11RenderTargetView* GetRenderTarget();

	ID3D11DepthStencilState* GetAddDepthState();
	ID3D11DepthStencilState* GetSubDepthState();

private:
	RenderToTexture(const RenderToTexture& _krInstanceToCopy);
	const RenderToTexture& operator =(const RenderToTexture& _krInstanceToCopy);

private:
	bool m_bHasRenderTarget;
	bool m_bHasDepthStencil;

	ID3D11Texture2D* m_pRenderTexture;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11ShaderResourceView* m_pShaderResourceView;

	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11ShaderResourceView* m_pDepthResourceView;

	//Depth states
	ID3D11DepthStencilState* m_pAddDepthState;
	ID3D11DepthStencilState* m_pSubDepthState;

};

#endif // __RenderToTexture_H__
