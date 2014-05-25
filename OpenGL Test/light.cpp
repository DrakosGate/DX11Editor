//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   light.cpp
//  Description :   Code for Class CLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes

// This Include
#include "light.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CLight constructor
*
* @author Christopher Howlett
*
*/
CLight::CLight()
:	m_fSpecularPower(0.0f)
,	m_bIsOn(false)
{

}
/**
*
* CLight destructor
*
* @author Christopher Howlett
*
*/
CLight::~CLight()
{

}
/**
*
* CLight Initialisation
*
* @author Christopher Howlett
* @param _vecDir Light Direction
* @param _vecColour Light Colour
* @param _fSpecularPower Light specular power
*
*/
void 
CLight::Initialise(D3DXVECTOR3& _rVecDir, D3DXCOLOR& _rVecColour, float _fSpecularPower, ELightType _eLightType)
{
	D3DXVec3Normalize(&m_vecDirection, &_rVecDir);
	m_vecColour = _rVecColour;
	m_fSpecularPower = _fSpecularPower;

	m_eLightType = _eLightType;
}
/**
*
* Processes Parent functionality on this gameobject
*
* @author Christopher Howlett
* @param Game time elapsed
*
*/
void 
CLight::ProcessParent(float _fDeltaTime)
{
	//if(m_bHasParent)
	//{
	//	//Move object relative to parent
	//	m_vecPosition = m_pParent->GetPosition();
	//	m_vecDirection = m_pParent->GetForward();
	//}
}
/**
*
* CLight SetDirection
*
* @author Christopher Howlett
* @param _rVecDir Light Direction
*
*/
void 
CLight::SetPosition(D3DXVECTOR3& _rVecPosition)
{
	m_vecWorldPosition = _rVecPosition;
}
/**
*
* CLight GetOffset
*
* @author Christopher Howlett
* @return Returns the offset of this light
*
*/
D3DXVECTOR3&
CLight::GetOffset()
{
	return m_vecOffsetPosition;
}
/**
*
* CLight GetDirection
*
* @author Christopher Howlett
* @return Returns light Direction
*
*/
D3DXVECTOR3& 
CLight::GetPosition()
{
	return m_vecWorldPosition;
}
/**
*
* CLight SetDirection
*
* @author Christopher Howlett
* @param _rVecDir Light Direction
*
*/
void 
CLight::SetDirection(D3DXVECTOR3& _rVecDir)
{
	D3DXVec3Normalize(&m_vecDirection, &_rVecDir);
}
/**
*
* CLight GetDirection
*
* @author Christopher Howlett
* @return Returns light Direction
*
*/
D3DXVECTOR3& 
CLight::GetDirection()
{
	return m_vecDirection;
}
/**
*
* CLight SetColour
*
* @author Christopher Howlett
* @param _rVecColour Colour to be set
*
*/
void 
CLight::SetColour(D3DXCOLOR& _rVecColour)
{
	m_vecColour = _rVecColour;
}
/**
*
* CLight GetColour
*
* @author Christopher Howlett
* @return Returns light Colour 
*
*/
D3DXCOLOR& 
CLight::GetColour()
{
	return m_vecColour;
}
/**
*
* CLight GetSpecularPower
*
* @author Christopher Howlett
* @return Returns specular power
*
*/
float 
CLight::GetSpecularPower() const
{
	return m_fSpecularPower;
}
/**
*
* CLight IsOn
*
* @author Christopher Howlett
* @return Returns if light is turned on
*
*/
bool 
CLight::IsOn() const
{
	return m_bIsOn;
}
/**
*
* CLight ToggleIsOn
*
* @author Christopher Howlett
* @param _bIsOn Light is on
*
*/
void 
CLight::ToggleIsOn(bool _bIsOn)
{
	m_bIsOn = _bIsOn;
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
CLight::GetLightInfo()
{
	m_tLightInfo = TLightInfo(m_vecWorldPosition, m_vecDirection, D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.1f), m_vecColour, m_vecColour, D3DXVECTOR3(1.0f, 0.2f, 0.05f), m_fSpecularPower, 1000.0f, m_eLightType); 
	return m_tLightInfo;
}