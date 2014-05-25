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
	CSpotLight* AddSpot(D3DXVECTOR3& _rVecPosition, D3DXVECTOR3& _rVecDirection, D3DXCOLOR& _rVecColour, D3DXVECTOR3& _rAttenuation, float _fCutOff, float _fSpecularPower);
	CPointLight* AddPoint(D3DXVECTOR3& _rVecPosition, D3DXCOLOR& _rVecColour, D3DXVECTOR3& _rAttenuation, float _fSpecularPower);
	CDirectionalLight* AddDirectional(D3DXVECTOR3& _rVecDir, D3DXCOLOR& _rVecColour, float _fSpecularPower);

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

	int m_iLightCount;
};

#endif //__LIGHTMANAGER_H__
