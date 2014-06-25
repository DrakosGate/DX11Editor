//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   spotlight.cpp
//  Description :   Code for Class spotlight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes

// This Include
#include "spotlight.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CSpotLight constructor
*
* @author Christopher Howlett
*
*/
CSpotLight::CSpotLight()
{

}
/**
*
* CSpotLight destructor
*
* @author Christopher Howlett
*
*/
CSpotLight::~CSpotLight()
{

}

/**
*
* CSpotLight Initialisation
*
* @author Christopher Howlett
* @param _rVecPosition Light Position
* @param _vecColour Light Colour
* @param _rAttenuation Light Atten
* @param _fCutoff Spot cutoff
* @param _fSpecularPower Light specular power
*
*/
bool 
CSpotLight::Initialise(D3DXVECTOR3& _rVecPosition, D3DXVECTOR3& _rVecDirection, D3DXCOLOR& _rVecColour, D3DXVECTOR3& _rAttenuation, float _fCutOff, float _fSpecularPower, ELightType _eLightType)
{
	m_vecOffsetPosition = _rVecPosition;
	m_vecWorldPosition = _rVecPosition;
	m_vecColour = _rVecColour;
	m_vecAttenuation = _rAttenuation;
	m_fSpecularPower = _fSpecularPower;
	m_fCutOff = _fCutOff;
	D3DXVec3Normalize(&m_vecDirection, &_rVecDirection);

	m_eLightType = _eLightType;

	return true;
}
/**
*
* CSpotLight GetAttenuation
*
* @author Christopher Howlett
* @return Returns Attenuation
*
*/
D3DXVECTOR3& 
CSpotLight::GetAttenuation()
{
	return m_vecAttenuation;
}
/**
*
* CSpotLight SetAttenuation
*
* @author Christopher Howlett
* @param Sets Attenuation
*
*/
void 
CSpotLight::SetAttenuation(D3DXVECTOR3& _rAttenuation)
{
	m_vecAttenuation = _rAttenuation;
}
/**
*
* CSpotLight GetCutoff
*
* @author Christopher Howlett
* @return Returns Cutoff
*
*/
float 
CSpotLight::GetCutoff() const
{
	return m_fCutOff;
}
/**
*
* CSpotLight SetCutoff
*
* @author Christopher Howlett
* @param _fCutoff Sets Cutoff
*
*/
void 
CSpotLight::SetCutoff(float _fCutoff)
{
	m_fCutOff = _fCutoff;
}
/**
*
* CLight GetLightInfo
*
* @author Christopher Howlett
* @return Returns Light information structure
*
*/
TLightInfo& 
CSpotLight::GetLightInfo()
{
	m_tLightInfo = TLightInfo(m_vecWorldPosition, m_vecDirection, D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f), m_vecColour, m_vecColour, m_vecAttenuation, m_fSpecularPower, 1000.0f, m_eLightType); 
	return m_tLightInfo;
}