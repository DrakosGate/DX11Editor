//
//  File Name   :   CSpotLight.h
//  Description :   Class of CSpotLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __SPOTLIGHT_H__
#define __SPOTLIGHT_H__

// Library Includes

// Local Includes
#include "light.h"

// Types

// Constants

// Prototypes

class CSpotLight : public CLight
{
public:
	CSpotLight();
	virtual ~CSpotLight();

	virtual bool Initialise(D3DXVECTOR3& _rVecPosition, D3DXVECTOR3& _rVecDirection, D3DXCOLOR& _rVecColour, D3DXVECTOR3& _rAttenuation, float _fCutOff, float _fSpecularPower, ELightType _eLightType);
	
	D3DXVECTOR3& GetAttenuation();
	void SetAttenuation(D3DXVECTOR3& _rAttenuation);
	float GetCutoff() const;
	void SetCutoff(float _fCutoff);
	virtual TLightInfo& GetLightInfo();

private:
	CSpotLight(const CSpotLight& _krInstanceToCopy);
	const CSpotLight& operator =(const CSpotLight& _krInstanceToCopy);

private:
	//D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecAttenuation;
	float m_fCutOff;
};
#endif //__SPOTLIGHT_H__