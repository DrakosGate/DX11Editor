//
//  File Name   :   light.h
//  Description :   Class of CLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

// Library Includes
#include "mathlibrary.h"

// Local Includes

// Types
struct TLightInfo
{
	TLightInfo()
	{
		ZeroMemory( this, sizeof( TLightInfo ) );
	}
	TLightInfo( Math::Vector3& _rPos, Math::Vector3& _rDir, Math::Colour& _rAmbient, Math::Colour& _rDiffuse, Math::Colour& _rSpecular, Math::Vector3& _rAtt, float _fSpotPow, float _fRange, int _iLightType )
	{
		ZeroMemory( this, sizeof( TLightInfo ) );
		pos = _rPos;
		dir = _rDir;
		ambient = _rAmbient;
		diffuse = _rDiffuse;
		specular = _rSpecular;
		att = _rAtt;
		spotPow = _fSpotPow;
		range = _fRange;
		iLightType = _iLightType;
	}
	Math::Vector3 pos;	//24
	float pad[2];			//4

	Math::Vector3 dir;	//12
	float pad2[2];			//4

	Math::Colour ambient;	//16

	Math::Colour diffuse;	//16

	Math::Colour specular;	//16

	Math::Vector3 att;	//12
	float pad3[2];			//4

	float spotPow;		//4
	float range;		//4
	int iLightType;		//4
	float pad4;			//4
};
enum ELightType
{
	LIGHT_INVALID = -1,
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_MAX
};

// Constants

// Prototypes

class CLight
{
public:
	CLight();
	virtual ~CLight();

	virtual void Initialise( Math::Vector3& _rVecDir, Math::Colour& _rVecColour, float _fSpecularPower, ELightType _eLightType );
	virtual void ProcessParent( float _fDeltaTime );

	virtual void SetPosition( Math::Vector3& _rVecPosition );
	virtual Math::Vector3& GetOffset();
	virtual Math::Vector3& GetPosition();
	virtual void SetDirection( Math::Vector3& _rVecDir );
	virtual Math::Vector3& GetDirection();
	virtual void SetColour( Math::Colour& _rVecColour );
	virtual Math::Colour& GetColour();
	virtual float GetSpecularPower() const;

	virtual bool IsOn() const;
	virtual void ToggleIsOn( bool _bIsOn );
	virtual TLightInfo& GetLightInfo();

private:
	CLight( const CLight& _kr );
	CLight& operator= ( const CLight& _rhs );

protected:
	Math::Vector3 m_vecOffsetPosition;
	Math::Vector3 m_vecWorldPosition;
	Math::Vector3 m_vecDirection;
	Math::Colour m_vecColour;
	float m_fSpecularPower;
	bool m_bIsOn;
	TLightInfo m_tLightInfo;

	ELightType m_eLightType;
};

#endif //__LIGHT_H__