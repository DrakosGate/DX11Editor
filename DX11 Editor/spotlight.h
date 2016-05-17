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
#include "mathlibrary.h"

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

	virtual bool Initialise(Math::Vector3& _rVecPosition, Math::Vector3& _rVecDirection, Math::Colour& _rVecColour, Math::Vector3& _rAttenuation, float _fCutOff, float _fSpecularPower, ELightType _eLightType);
	
	Math::Vector3& GetAttenuation();
	void SetAttenuation(Math::Vector3& _rAttenuation);
	float GetCutoff() const;
	void SetCutoff(float _fCutoff);
	virtual TLightInfo& GetLightInfo();

private:
	CSpotLight(const CSpotLight& _krInstanceToCopy);
	const CSpotLight& operator =(const CSpotLight& _krInstanceToCopy);

private:
	//Math::Vector3 m_vecPosition;
	Math::Vector3 m_vecAttenuation;
	float m_fCutOff;
};
#endif //__SPOTLIGHT_H__