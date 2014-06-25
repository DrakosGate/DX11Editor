//
//  File Name   :   camera.h
//  Description :   Class of Camera
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

// Library Includes
#include <D3DX10math.h>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes
class CCubeInterface;

class CCamera
{
	//Member functions
public:
	CCamera();
	~CCamera();

	bool Initialise(float _fMovementSpeed, float _fRotationSpeed, float _fOrbitSpeed, int _iWindowWidth, int _iWindowHeight, bool _bUsesPerspective);
	void Process(float _fDeltaTime);
	bool ProcessInput(TInputStruct* _pKeys, D3DXVECTOR2& _vecMouseDirection, bool _bScrollCamera, float _fDT);

	TRay& GetMouseRay(D3DXVECTOR2& _rMousePoint);
	void CreateProjectionMatrix(float _fAspect);
	void SetPosition(D3DXVECTOR3& _vecPosition);
	void SetLook(D3DXVECTOR3& _vecLook);
	void SetTargetLook(D3DXVECTOR3& _vecTargetLook);
	void CalculateViewFrustum();
	bool IsInFrustum(D3DXVECTOR3 &_vec3Position, float _fRadius);

	D3DXMATRIX& GetWorldMatrix();
	D3DXMATRIX& GetViewMatrix();
	D3DXMATRIX& GetProjection();
	D3DXVECTOR3& GetPosition();
	D3DXVECTOR3& GetVelocity();
	D3DXVECTOR3& GetRotationSpeed();
	D3DXVECTOR3& GetLook();

	
private:
	CCamera(const CCamera& _krInstanceToCopy);
	const CCamera& operator =(const CCamera& _krInstanceToCopy);

	//Member variables
private:
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecVelocity;
	D3DXVECTOR3 m_vecTargetLook;
	D3DXVECTOR3 m_vecRotationSpeed;
	D3DXVECTOR3 m_vecLook;
	D3DXVECTOR3 m_vecUp;
	D3DXVECTOR3 m_vecRight;
	D3DXVECTOR3 m_vecRotation;
	D3DXVECTOR2 m_vecMouseDirection;
	D3DXQUATERNION m_quatRotation;

	D3DXPLANE* m_pViewFrustum;

	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fCurrentZoomHeight;

	TInputStruct* m_pPlayerInput;
	D3DXVECTOR2 m_vecCameraRotationOrigin;
	bool m_bUsesPerspective;
	int m_iWindowWidth;
	int m_iWindowHeight;

	TRay m_tMouseRay;
	bool m_bMouseRayCalculatedThisFrame;
};



#endif //CAMERA_H__