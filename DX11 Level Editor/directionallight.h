//
//  File Name   :   CDirectionalLight.h
//  Description :   Class of CDirectionalLight
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __DIRECTIONALLIGHT_H__
#define __DIRECTIONALLIGHT_H__

// Library Includes

// Local Includes
#include "light.h"

// Types

// Constants

// Prototypes

class CDirectionalLight : public CLight
{
public:
	CDirectionalLight();
	virtual ~CDirectionalLight();
	
	virtual TLightInfo& GetLightInfo();

private:
	CDirectionalLight(const CDirectionalLight& _kr);
	CDirectionalLight& operator= (const CDirectionalLight& _rhs);

private:
};

#endif //__DIRECTIONALLIGHT_H__