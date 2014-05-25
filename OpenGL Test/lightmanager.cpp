//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   lightmanager.cpp
//  Description :   Definition of CLightManager class member functions
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "spotlight.h"
#include "pointlight.h"
#include "directionallight.h"

// This Include
#include "lightmanager.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CLightManager constructor
*
* @author Christopher Howlett
*
*/
CLightManager::CLightManager()
: m_iLightCount(0)
{
}

/**
*
* CLightManager destructor
*
* @author Christopher Howlett
*
*/
CLightManager::~CLightManager()
{
	for(unsigned int iLight = 0; iLight < m_vSpotLights.size(); ++iLight)
	{
		delete m_vSpotLights[iLight];
	}
	m_vSpotLights.clear();
	for(unsigned int iLight = 0; iLight < m_vPointLights.size(); ++iLight)
	{
		delete m_vPointLights[iLight];
	}
	m_vPointLights.clear();
	for(unsigned int iLight = 0; iLight < m_vDirectionalLights.size(); ++iLight)
	{
		delete m_vDirectionalLights[iLight];
	}
	m_vDirectionalLights.clear();
}
	
/**
*
* CLightManager Initialise function
*
* @author Christopher Howlett
*
*/
bool 
CLightManager::Initialise()
{
	return true;
}
CSpotLight* 
CLightManager::AddSpot(D3DXVECTOR3& _rVecPosition, D3DXVECTOR3& _rVecDirection, D3DXCOLOR& _rVecColour, D3DXVECTOR3& _rAttenuation, float _fCutOff, float _fSpecularPower)
{
	CSpotLight* pNewSpot = new CSpotLight;
	pNewSpot->Initialise(_rVecPosition, _rVecDirection, _rVecColour, _rAttenuation, _fCutOff, _fSpecularPower, LIGHT_SPOT);
	m_vSpotLights.push_back(pNewSpot);
	++m_iLightCount;
	return pNewSpot;
}
CPointLight*
CLightManager::AddPoint(D3DXVECTOR3& _rVecPosition, D3DXCOLOR& _rVecColour, D3DXVECTOR3& _rAttenuation, float _fSpecularPower)
{
	CPointLight* pNewPoint = new CPointLight;
	pNewPoint->Initialise(_rVecPosition, _rVecColour, _rAttenuation, _fSpecularPower, LIGHT_POINT);
	m_vPointLights.push_back(pNewPoint);
	++m_iLightCount;
	return pNewPoint;
}
CDirectionalLight*
CLightManager::AddDirectional(D3DXVECTOR3& _rVecDir, D3DXCOLOR& _rVecColour, float _fSpecularPower)
{
	CDirectionalLight* pNewDirectional = new CDirectionalLight;
	pNewDirectional->Initialise(_rVecDir, _rVecColour, _fSpecularPower, LIGHT_DIRECTIONAL);
	m_vDirectionalLights.push_back(pNewDirectional);
	++m_iLightCount;
	return pNewDirectional;
}
int 
CLightManager::GetLightCount(ELightType _eLightType) const
{
	int iLightCount = 0;
	switch(_eLightType)
	{
	case LIGHT_SPOT:
		{
			iLightCount = m_vSpotLights.size();
		}
		break;
	case LIGHT_POINT:
		{
			iLightCount = m_vPointLights.size();
		}
		break;
	case LIGHT_DIRECTIONAL:
		{
			iLightCount = m_vDirectionalLights.size();
		}
		break;
	default:
		break;
	}
	return iLightCount;
}
TLightInfo* 
CLightManager::GetLightInfo(ELightType _eLightType)
{
	return 0;
}
CSpotLight* 
CLightManager::GetSpot(int _iIndex)
{
	return m_vSpotLights[_iIndex];
}
CPointLight* 
CLightManager::GetPoint(int _iIndex)
{
	return m_vPointLights[_iIndex];
}
CDirectionalLight* 
CLightManager::GetDirectional(int _iIndex)
{
	return m_vDirectionalLights[_iIndex];
}
int 
CLightManager::GetLightCount() const
{
	return m_iLightCount;
}