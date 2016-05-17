//
//  File Name   :   RenderEntity.h
//  Description :   Class of RenderEntity
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __RENDERENTITY_H__
#define __RENDERENTITY_H__

// Library Includes
#include <vector>

// Local Includes
#include "vertex.h"
#include "defines.h"

// Types
class RenderEntity;
class CLight;

struct TEntityNode
{
	TEntityNode()
	: pEntity(0)
	, pParent(0)
	{}
	TEntityNode(RenderEntity* _pEntity, TEntityNode* _pParent)
	{
		pEntity = _pEntity;
		pParent = _pParent;
	}
	void Clear()
	{
		pEntity = 0;
		pParent = 0;
		for (unsigned int iChild = 0; iChild < vecChildren.size(); ++iChild)
		{
			vecChildren[iChild]->Clear();
			vecChildren[iChild] = 0;
		}
		vecChildren.clear();
	}
	RenderEntity* pEntity;
	TEntityNode* pParent;
	std::vector<TEntityNode*> vecChildren;
	std::vector<CLight*> vecLights;
};

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11ShaderResourceView;
struct ID3D11Buffer;

class CBoundingBox;
class Camera;
class CShader;

class RenderEntity
{
//Member functions
public:
	RenderEntity();
	virtual ~RenderEntity();

	virtual bool Initialise(ID3D11Device* _pDevice, float _fScale);
	virtual void Process(float _fDeltaTime, Math::Matrix* _pParentMatrix);
	virtual void Draw(ID3D11DeviceContext* _pDevice);
	
	virtual void CreateVertexBuffer(ID3D11Device* _pDevice);
	virtual void CreateIndexBuffer(ID3D11Device* _pDevice);
	virtual TEntityNode* CreateNode(TEntityNode* _pParentNode);
	virtual TEntityNode* GetNode() const;

	virtual Math::Vector3& GetPosition();
	virtual Math::Vector3& GetRotation();
	virtual Math::Vector3& GetScale();
	virtual Math::Vector3& GetLocalScale();
	virtual Math::Vector3& GetForward();
	virtual Math::Matrix& GetWorld();
	virtual std::string& GetEntityType();
	virtual void SetRotation(Math::Vector3& _rVecRotation);
	virtual void SetPosition(Math::Vector3& _rVecPosition);
	virtual void SetForward(Math::Vector3& _rVecForward);
	virtual void SetWorldMatrix(Math::Matrix& _rWorld);
	virtual void SetScale(Math::Vector3& _rVecScale);
	virtual void SetLocalScale(Math::Vector3& _rLocalScale);
	virtual void SetEntityType(std::string& _sType);

	virtual ID3D11ShaderResourceView* GetDiffuseMap();
	virtual ID3D11ShaderResourceView* GetNormalMap();
	virtual void SetDiffuseMap(ID3D11ShaderResourceView* _pDiffuseMap);
	virtual void SetNormalMap(ID3D11ShaderResourceView* _pNormalMap);
	virtual void SendTextureDataToShader(ID3D11DeviceContext* _pDevContext);

	virtual TVertex* GetVertexData(int _iVertex);
	virtual int GetVertexCount() const;
	virtual void SetObjectShader(CShader* _pObjectShader);
	virtual CShader* GetObjectShader();

	virtual void ProcessBillboard(Camera* _pCurrentCamera, Math::Matrix& _rBillboardMat);
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
	virtual bool HasCollided(RenderEntity* _pOtherEntity);

	virtual bool DoDraw() const;
	virtual void SetDoDraw(bool _bDoDraw);

private:
	RenderEntity(const RenderEntity& _krInstanceToCopy);
	const RenderEntity& operator =(const RenderEntity& _krInstanceToCopy);
	
//Member variables
protected:
	TEntityNode* m_pNode;
	CShader* m_pObjectShader;
	CBoundingBox* m_pBoundingBox;
	float m_fRadius;
	float m_fBillboardFlip;

	Math::Matrix m_matWorld;
	Math::Vector3 m_vecPosition;
	Math::Vector3 m_vecRotation;
	Math::Vector3 m_vecScale;
	Math::Vector3 m_vecLocalScale;

	Math::Vector3 m_vecLook;
	Math::Vector3 m_vecUp;
	Math::Vector3 m_vecRight;
	Math::Quaternion m_quatRot;
	std::string m_sEntityType;

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