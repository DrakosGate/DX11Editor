//
//  File Name   :   tile.h
//  Description :   Class of CPrefab
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __PREFAB_H__
#define __PREFAB_H__

// Library Includes
#include <D3DX10.h>

// Local Includes
#include "defines.h"
#include "renderentity.h"

// Types

// Constants

// Prototypes
class CModel;

class CPrefab : public CRenderEntity
{
public:
	CPrefab();
	~CPrefab();
	
	virtual bool Initialise(ID3D11Device* _pDevice, float _fScale);
	virtual void Draw(ID3D11DeviceContext* _pDevice);
	
	virtual void SetModel(CModel* _pModel);
	
	void SetTileType(ETileType _eTileType);
	ETileType GetTileType() const;

	bool IsCollidable() const;
	void ToggleIsCollidable(bool _bIsCollidable);
	bool CheckPointCollision(D3DXVECTOR3& _rPoint);

	virtual void SetScale(D3DXVECTOR3& _rVecScale);
	
protected:
	CModel* m_pModel;
	ETileType m_eTileType;
	float m_fBoundingBoxPercentage;

	bool m_bIsCollidable;
};


#endif //PREFAB_H__