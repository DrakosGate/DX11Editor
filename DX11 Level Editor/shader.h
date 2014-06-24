//
//  File Name   :   shader.h
//  Description :   Class of CShader
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

// Library Includes
#include <D3DX10.h>

// Local Includes
#include "defines.h"
#include "light.h"

// Types
enum EShaderBuffer
{
	BUFFER_INVALID = -1,
	BUFFER_MATRICES,
	BUFFER_LIGHTING,
	BUFFER_MAX
};

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;

class CCamera;
class CLightManager;

class CShader
{
	//Shader structures
	struct TMatBuffer
	{
		D3DXMATRIX matWorld;
		D3DXMATRIX matView;
		D3DXMATRIX matProj;
	};
	struct TLightBuffer
	{
		TLightInfo lightInfo[MAX_LIGHTS];		//112 bytes

		D3DXVECTOR3 vecCameraPos;	//12
		int iActiveLightCount;		//4
	};

//Member functions
public:
	CShader();
	virtual ~CShader();

	virtual bool Initialise(ID3D11Device* _pDevice);
	void CompileVertexShader(ID3D11Device* _pDevice, wchar_t* _pcVSFilename, char* _pcVSFunction);
	void CompilePixelShader(ID3D11Device* _pDevice, wchar_t* _pcPSFilename, char* _pcPSFunction);
	void CompileGeometryShader(ID3D11Device* _pDevice, wchar_t* _pcGSFilename, char* _pcGSFunction);
	
	void SendWVPMatrixData(ID3D11DeviceContext* _pDeviceContext, D3DXMATRIX* _pWorld, D3DXMATRIX* _pView, D3DXMATRIX* _pProjection);
	void SendLightInformation(ID3D11DeviceContext* _pDeviceContext, CLightManager* _pLightManager, CCamera* _pActiveCamera);
	
	ID3D11VertexShader* GetVertexShader() const;
	ID3D11GeometryShader* GetGeometryShader() const;
	ID3D11PixelShader* GetPixelShader() const;
	ID3D10Blob* GetShaderBlob() const;

private:
	CShader(const CShader& _krInstanceToCopy);
	const CShader& operator =(const CShader& _krInstanceToCopy);
	
//Member variables
protected:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11GeometryShader* m_pGeometryShader;
	ID3D11Buffer* m_pShaderBuffers[BUFFER_MAX];

	//Buffer info
	ID3D10Blob* pVertexShaderBuffer = 0;
	ID3D10Blob* pPixelShaderBuffer = 0;
	ID3D10Blob* pGeometryShaderBuffer = 0;
};

#endif // __SHADER_H__