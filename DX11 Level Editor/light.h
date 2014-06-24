//
//  File Name   :   light.h
//  Description :   Class of CLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

// Library Includes
#include <D3DX10.h>

// Local Includes

// Types
struct TLightInfo
{
	TLightInfo()
	{
		ZeroMemory(this, sizeof(TLightInfo));
	}
	TLightInfo(D3DXVECTOR3& _rPos, D3DXVECTOR3& _rDir, D3DXCOLOR& _rAmbient, D3DXCOLOR& _rDiffuse, D3DXCOLOR& _rSpecular, D3DXVECTOR3& _rAtt, float _fSpotPow, float _fRange, int _iLightType)
	{
		ZeroMemory(this, sizeof(TLightInfo));
		pos = _rPos;
		dir = _rDir;
		ambient = _rAmbient;
		diffuse = _rDiffuse;
		specular = _rSpecular;
		att = _rAtt;
		spotPow = _fSpotPow;
		range = _fRange;
		iLightType = _iLightType;
	}
	D3DXVECTOR3 pos;	//12
	float pad;			//4

	D3DXVECTOR3 dir;	//12
	float pad2;			//4

	D3DXCOLOR ambient;	//16

	D3DXCOLOR diffuse;	//16

	D3DXCOLOR specular;	//16

	D3DXVECTOR3 att;	//12
	float pad3;			//4

	float spotPow;		//4
	float range;		//4
	int iLightType;		//4
	float pad4;			//4
};
enum ELightType
{
	LIGHT_INVALID = -1,
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_MAX
};

// Constants

// Prototypes

class CLight
{
public:
	CLight();
	virtual ~CLight();

	virtual void Initialise(D3DXVECTOR3& _rVecDir, D3DXCOLOR& _rVecColour, float _fSpecularPower, ELightType _eLightType);
	virtual void ProcessParent(float _fDeltaTime);
	
	virtual void SetPosition(D3DXVECTOR3& _rVecPosition);
	virtual D3DXVECTOR3& GetOffset();
	virtual D3DXVECTOR3& GetPosition();
	virtual void SetDirection(D3DXVECTOR3& _rVecDir);
	virtual D3DXVECTOR3& GetDirection();
	virtual void SetColour(D3DXCOLOR& _rVecColour);
	virtual D3DXCOLOR& GetColour();
	virtual float GetSpecularPower() const;

	virtual bool IsOn() const;
	virtual void ToggleIsOn(bool _bIsOn);
	virtual TLightInfo& GetLightInfo();

private:
	CLight(const CLight& _kr);
	CLight& operator= (const CLight& _rhs);

protected:
	D3DXVECTOR3 m_vecOffsetPosition;
	D3DXVECTOR3 m_vecWorldPosition;
	D3DXVECTOR3 m_vecDirection;
	D3DXCOLOR m_vecColour;
	float m_fSpecularPower;
	bool m_bIsOn;
	TLightInfo m_tLightInfo;

	ELightType m_eLightType;
};

#endif //__LIGHT_H__