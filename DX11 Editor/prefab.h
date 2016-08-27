//
//  File Name   :   tile.h
//  Description :   Class of Prefab
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __PREFAB_H__
#define __PREFAB_H__

// Library Includes
// Local Includes
#include "defines.h"
#include "renderentity.h"

// Types

// Constants

// Prototypes
class Model;

class Prefab : public RenderEntity
{
public:
	Prefab();
	~Prefab();
	
	virtual bool Initialise(ID3D11Device* _pDevice, float _fScale);
	virtual void Draw(ID3D11DeviceContext* _pDevice);
	
	virtual void SetModel(Model* _pModel);
	
	void SetTileType(ETileType _eTileType);
	ETileType GetTileType() const;

	bool IsCollidable() const;
	void ToggleIsCollidable(bool _bIsCollidable);
	bool CheckPointCollision(Math::Vector3& _rPoint);

	virtual void SetScale(Math::Vector3& _rVecScale);

protected:
	Model* m_pModel;
	ETileType m_eTileType;
	float m_fBoundingBoxPercentage;

	bool m_bIsCollidable;
};


#endif //PREFAB_H__