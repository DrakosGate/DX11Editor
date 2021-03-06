
// Library Includes

// Local Includes

// This Include
#include "pointlight.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CPointLight constructor
*
* @author Christopher Howlett
*
*/
CPointLight::CPointLight()
{

}
/**
*
* CPointLight destructor
*
* @author Christopher Howlett
*
*/
CPointLight::~CPointLight()
{

}
/**
*
* CPointLight Initialisation
*
* @author Christopher Howlett
* @param _rVecPosition Light Position
* @param _vecColour Light Colour
* @param _rAttenuation Light Atten
* @param _fSpecularPower Light specular power
*
*/
bool
CPointLight::Initialise( Math::Vector3& _rVecPosition, Math::Colour& _rVecColour, Math::Vector3& _rAttenuation, float _fSpecularPower, ELightType _eLightType )
{
	m_vecOffsetPosition = _rVecPosition;
	m_vecWorldPosition = _rVecPosition;
	m_vecColour = _rVecColour;
	m_vecAttenuation = _rAttenuation;
	m_fSpecularPower = _fSpecularPower;

	m_eLightType = _eLightType;

	return true;
}
/**
*
* CPointLight GetAttenuation
*
* @author Christopher Howlett
* @return Returns attenuation
*
*/
Math::Vector3&
CPointLight::GetAttenuation()
{
	return m_vecAttenuation;
}
/**
*
* CPointLight SetAttenuation
*
* @author Christopher Howlett
* @param _rAttenuation New attenuation
*
*/
void
CPointLight::SetAttenuation( Math::Vector3& _rAttenuation )
{
	m_vecAttenuation = _rAttenuation;
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
CPointLight::GetLightInfo()
{
	m_tLightInfo = TLightInfo( m_vecWorldPosition, Math::Vector3( 1.0f, 0.0f, 0.0f ), Math::Colour( 0.0f, 0.0f, 0.0f, 0.0f ), m_vecColour, Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f ), m_vecAttenuation, m_fSpecularPower, 10.0f, m_eLightType );
	return m_tLightInfo;
}