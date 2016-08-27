//
//  File Name   :   shader.h
//  Description :   Class of Shader
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

// Library Includes
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

class Camera;
class LightManager;

class Shader
{
	//Shader structures
	struct TMatBuffer
	{
		DirectX::XMMATRIX matWorld;
		DirectX::XMMATRIX matView;
		DirectX::XMMATRIX matProj;
	};
	struct TLightBuffer
	{
		TLightInfo lightInfo[MAX_LIGHTS];		//112 bytes

		Math::Vector3 veCameraPos;	//24
		int iActiveLightCount;		//4
		float pad;
	};

//Member functions
public:
	Shader();
	virtual ~Shader();

	virtual bool Initialise(ID3D11Device* _pDevice);
	void CompileVertexShader(ID3D11Device* _pDevice, wchar_t* _pcVSFilename, char* _pcVSFunction);
	void CompilePixelShader(ID3D11Device* _pDevice, wchar_t* _pcPSFilename, char* _pcPSFunction);
	void CompileGeometryShader(ID3D11Device* _pDevice, wchar_t* _pcGSFilename, char* _pcGSFunction);
	
	void SendWVPMatrixData(ID3D11DeviceContext* _pDeviceContext, Math::Matrix* _pWorld, Math::Matrix* _pView, Math::Matrix* _pProjection);
	void SendLightInformation(ID3D11DeviceContext* _pDeviceContext, LightManager* _pLightManager, Camera* _pActiveCamera);
	
	ID3D11VertexShader* GetVertexShader() const;
	ID3D11GeometryShader* GetGeometryShader() const;
	ID3D11PixelShader* GetPixelShader() const;
	ID3D10Blob* GetShaderBlob() const;

private:
	Shader(const Shader& _krInstanceToCopy);
	const Shader& operator =(const Shader& _krInstanceToCopy);
	
//Member variables
protected:
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11GeometryShader* m_pGeometryShader;
	ID3D11Buffer* m_pShaderBuffers[BUFFER_MAX];

	//Buffer info
	ID3D10Blob* pVertexShaderBuffer = nullptr;
	ID3D10Blob* pPixelShaderBuffer = nullptr;
	ID3D10Blob* pGeometryShaderBuffer = nullptr;
};

#endif // __SHADER_H__