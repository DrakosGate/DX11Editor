//
//  File Name   :   CRenderEntity.h
//  Description :   Class of CRenderEntity
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __RENDERENTITY_H__
#define __RENDERENTITY_H__

// Library Includes
#include <D3DX10math.h>

// Local Includes
#include "vertex.h"
#include "defines.h"

// Types

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11Buffer;

class CBoundingBox;
class CCamera;
class CShader;

class CRenderEntity
{
//Member functions
public:
	CRenderEntity();
	virtual ~CRenderEntity();

	virtual bool Initialise(ID3D11Device* _pDevice, float _fScale);
	virtual void Process(float _fDeltaTime);
	virtual void Draw(ID3D11DeviceContext* _pDevice);
	
	virtual void CreateVertexBuffer(ID3D11Device* _pDevice);
	virtual void CreateIndexBuffer(ID3D11Device* _pDevice);

	virtual D3DXVECTOR3& GetPosition();
	virtual D3DXVECTOR3& GetScale();
	virtual D3DXVECTOR3& GetForward();
	virtual D3DXMATRIX& GetWorld();
	virtual void SetRotation(D3DXVECTOR3& _rVecRotation);
	virtual void SetPosition(D3DXVECTOR3& _rVecPosition);
	virtual void SetForward(D3DXVECTOR3& _rVecForward);
	virtual bool ProcessInput(TInputStruct& _pKeys, float _fDT);
	virtual void SetWorldMatrix(D3DXMATRIX& _rWorld);
	virtual void SetScale(D3DXVECTOR3& _rVecScale);

	virtual ID3D11ShaderResourceView* GetDiffuseMap();
	virtual ID3D11ShaderResourceView* GetNormalMap();
	virtual void SetDiffuseMap(ID3D11ShaderResourceView* _pDiffuseMap);
	virtual void SendTextureDataToShader(ID3D11DeviceContext* _pDevContext);

	virtual int GetVertexCount() const;
	virtual void SetObjectShader(CShader* _pObjectShader);
	virtual CShader* GetObjectShader();

	virtual void ProcessBillboard(CCamera* _pCurrentCamera, D3DXMATRIX& _rBillboardMat);
	virtual bool IsBillboarded() const;
	virtual void ToggleBillboarded(bool _bIsBillboard);
	virtual bool IsShadowed() const;
	virtual void ToggleShadowed(bool _bIsShadowed);
	virtual bool IsTransparent() const;
	virtual void ToggleTransparency(bool _bIsTransparent);
	virtual void CalculateBoundingBox();
	virtual CBoundingBox* GetBoundingBox();
	virtual float GetRadius() const;
	virtual void SetRadius(float _fRadius);
	virtual bool HasCollided(CRenderEntity* _pOtherEntity);

	virtual bool DoDraw() const;
	virtual void SetDoDraw(bool _bDoDraw);

private:
	CRenderEntity(const CRenderEntity& _krInstanceToCopy);
	const CRenderEntity& operator =(const CRenderEntity& _krInstanceToCopy);
	
//Member variables
protected:
	CShader* m_pObjectShader;
	CBoundingBox* m_pBoundingBox;
	float m_fRadius;
	float m_fBillboardFlip;

	D3DXMATRIX m_matWorld;
	D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecRotation;
	D3DXVECTOR3 m_vecScale;
	D3DXVECTOR3 m_vecLook;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecRight;
	D3DXQUATERNION m_quatRot;

	int m_iVertexCount;
	int m_iIndexCount;
	
	ID3D11Buffer* m_pVertexBuffer; // Vertex buffer
	ID3D11Buffer* m_pIndexBuffer; // Index  buffer

	ID3D11ShaderResourceView* m_pDiffuseMap;
	ID3D11ShaderResourceView* m_pNormalMap;

	TVertex* m_pVertices;
	unsigned int* m_pIndices;

	//Entity State
	bool m_bIsShadowed;
	bool m_bIsTransparent;
	bool m_bIsBillboarded;
	bool m_bDoDraw;
};

#endif // __RENDERENTITY_H__