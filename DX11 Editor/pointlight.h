//
//  File Name   :   CPointLight.h
//  Description :   Class of CPointLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__

// Library Includes

// Local Includes
#include "light.h"

// Types

// Constants

// Prototypes

class CPointLight : public CLight
{
public:
	CPointLight();
	virtual ~CPointLight();

	virtual bool Initialise(Math::Vector3& _rVecPosition, Math::Colour& _rVecColour, Math::Vector3& _rAttenuation, float _fSpecularPower, ELightType _eLightType);
	
	Math::Vector3& GetAttenuation();
	void SetAttenuation(Math::Vector3& _rAttenuation);
	virtual TLightInfo& GetLightInfo();

private:
	CPointLight(const CPointLight& _krInstanceToCopy);
	const CPointLight& operator =(const CPointLight& _krInstanceToCopy);

private:
	Math::Vector3 m_vecAttenuation;
};

#endif //__POINTLIGHT_H__