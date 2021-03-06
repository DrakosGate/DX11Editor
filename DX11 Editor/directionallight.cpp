
// Library Includes

// Local Includes

// This Include
#include "directionallight.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CDirectionalLight constructor
*
* @author Christopher Howlett
*
*/
CDirectionalLight::CDirectionalLight()
{

}
/**
*
* CDirectionalLight destructor
*
* @author Christopher Howlett
*
*/
CDirectionalLight::~CDirectionalLight()
{

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
CDirectionalLight::GetLightInfo()
{
	m_tLightInfo = TLightInfo( m_vecWorldPosition, m_vecDirection, Math::Colour( 0.1f, 0.1f, 0.1f, 0.1f ), m_vecColour, m_vecColour, Math::Vector3( 1.0f, 0.2f, 0.05f ), m_fSpecularPower, 1000.0f, m_eLightType );
	return m_tLightInfo;
}