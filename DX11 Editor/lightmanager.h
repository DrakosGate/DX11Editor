//
//  File Name   :   lightmanager.h
//  Description :   Declaration of CLightManager class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __LIGHTMANAGER_H__
#define __LIGHTMANAGER_H__

// Library Includes
#include <vector>

// Local Includes
#include "light.h"

// Types

// Constants

// Prototypes
class CLight;
class CSpotLight;
class CPointLight;
class CDirectionalLight;

class CLightManager
{
// Member functions
public:
	CLightManager();
	~CLightManager();
	
	bool Initialise();
	void DestroyLights();
	
	CSpotLight* AddSpot(Math::Vector3& _rVecPosition, Math::Vector3& _rVecDirection, Math::Colour& _rVecColour, Math::Vector3& _rAttenuation, float _fCutOff, float _fSpecularPower);
	CPointLight* AddPoint(Math::Vector3& _rVecPosition, Math::Colour& _rVecColour, Math::Vector3& _rAttenuation, float _fSpecularPower);
	CDirectionalLight* AddDirectional(Math::Vector3& _rVecDir, Math::Colour& _rVecColour, float _fSpecularPower);

	int GetLightCount(ELightType _eLightType) const;
	TLightInfo* GetLightInfo(ELightType _eLightType);

	CSpotLight* GetSpot(int _iIndex);
	CPointLight* GetPoint(int _iIndex);
	CDirectionalLight* GetDirectional(int _iIndex);

	int GetLightCount() const;

protected:
	CLightManager(const CLightManager& _kr);
	CLightManager& operator= (const CLightManager& _rhs);

// Member variables
protected:
	std::vector<CSpotLight*> m_vSpotLights;
	std::vector<CPointLight*> m_vPointLights;
	std::vector<CDirectionalLight*> m_vDirectionalLights;

};

#endif //__LIGHTMANAGER_H__
