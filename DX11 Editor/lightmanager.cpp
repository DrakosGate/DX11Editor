
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
* LightManager constructor
*
* @author Christopher Howlett
*
*/
LightManager::LightManager()
{
}

/**
*
* LightManager destructor
*
* @author Christopher Howlett
*
*/
LightManager::~LightManager()
{
	DestroyLights();
}

/**
*
* LightManager Initialise function
*
* @author Christopher Howlett
*
*/
bool
LightManager::Initialise()
{
	return true;
}
void
LightManager::DestroyLights()
{
	for( unsigned int iLight = 0; iLight < m_vSpotLights.size(); ++iLight )
	{
		delete m_vSpotLights[ iLight ];
	}
	m_vSpotLights.clear();
	for( unsigned int iLight = 0; iLight < m_vPointLights.size(); ++iLight )
	{
		delete m_vPointLights[ iLight ];
	}
	m_vPointLights.clear();
	for( unsigned int iLight = 0; iLight < m_vDirectionalLights.size(); ++iLight )
	{
		delete m_vDirectionalLights[ iLight ];
	}
	m_vDirectionalLights.clear();
}
CSpotLight*
LightManager::AddSpot( Math::Vector3& _rVecPosition, Math::Vector3& _rVecDirection, Math::Colour& _rVecColour, Math::Vector3& _rAttenuation, float _fCutOff, float _fSpecularPower )
{
	CSpotLight* pNewSpot = new CSpotLight;
	pNewSpot->Initialise( _rVecPosition, _rVecDirection, _rVecColour, _rAttenuation, _fCutOff, _fSpecularPower, LIGHT_SPOT );
	m_vSpotLights.push_back( pNewSpot );
	return pNewSpot;
}
CPointLight*
LightManager::AddPoint( Math::Vector3& _rVecPosition, Math::Colour& _rVecColour, Math::Vector3& _rAttenuation, float _fSpecularPower )
{
	CPointLight* pNewPoint = new CPointLight;
	pNewPoint->Initialise( _rVecPosition, _rVecColour, _rAttenuation, _fSpecularPower, LIGHT_POINT );
	m_vPointLights.push_back( pNewPoint );
	return pNewPoint;
}
CDirectionalLight*
LightManager::AddDirectional( Math::Vector3& _rVecDir, Math::Colour& _rVecColour, float _fSpecularPower )
{
	CDirectionalLight* pNewDirectional = new CDirectionalLight;
	pNewDirectional->Initialise( _rVecDir, _rVecColour, _fSpecularPower, LIGHT_DIRECTIONAL );
	m_vDirectionalLights.push_back( pNewDirectional );
	return pNewDirectional;
}
int
LightManager::GetLightCount( ELightType _eLightType ) const
{
	int iLightCount = 0;
	switch( _eLightType )
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
LightManager::GetLightInfo( ELightType _eLightType )
{
	return 0;
}
CSpotLight*
LightManager::GetSpot( int _iIndex )
{
	return m_vSpotLights[ _iIndex ];
}
CPointLight*
LightManager::GetPoint( int _iIndex )
{
	return m_vPointLights[ _iIndex ];
}
CDirectionalLight*
LightManager::GetDirectional( int _iIndex )
{
	return m_vDirectionalLights[ _iIndex ];
}
int
LightManager::GetLightCount() const
{
	return ( m_vPointLights.size() + m_vSpotLights.size() + m_vDirectionalLights.size() );
}