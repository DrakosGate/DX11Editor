//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   camera.cpp
//  Description :   Code for Class camera
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "defines.h"

// This Include
#include "camera.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CCamera class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CCamera::CCamera()
: m_fMovementSpeed(0.0f)
, m_fRotationSpeed(0.0f)
, m_pViewFrustum(0)
, m_bUsesPerspective(true)
, m_iWindowWidth(0)
, m_iWindowHeight(0)
, m_fCurrentZoomHeight(0.0f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixIdentity(&m_matWorld);

	m_vecMouseDirection *= 0.0f;
	m_vecRotationSpeed *= 0.0f;
	m_vecRotation *= 0.0f;
	m_vecVelocity *= 0.0f;
	m_vecPosition *= 0.0f;

	m_vecCameraRotationOrigin *= 0.0f;
	
	m_vecLook.x = 0.0f;
	m_vecLook.y = 0.0f;
	m_vecLook.z = -1.0f;
	m_vecTargetLook = m_vecLook;
	m_vecUp.x = 0.0f;
	m_vecUp.y = 1.0f;
	m_vecUp.z = 0.0f;
	m_vecRight.x = 1.0f;
	m_vecRight.y = 0.0f;
	m_vecRight.z = 0.0f;
	
}
/**
*
* CCamera class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CCamera::~CCamera()
{
	if(m_pViewFrustum)
	{
		delete m_pViewFrustum;
		m_pViewFrustum = 0;
	}
}
/**
*
* CCamera class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fMovementSpeed Camera MovementSpeed
* @param _fRotationSpeed camera rotation speed
* @return Returns true
*
*/
bool
CCamera::Initialise(float _fMovementSpeed, float _fRotationSpeed, float _fOrbitSpeed, int _iWindowWidth, int _iWindowHeight, bool _bUsesPerspective)
{
	m_fMovementSpeed = _fMovementSpeed;
	m_fRotationSpeed = _fRotationSpeed;
	m_bUsesPerspective = _bUsesPerspective;
	m_iWindowWidth = _iWindowWidth;
	m_iWindowHeight = _iWindowHeight;

	m_pViewFrustum = new D3DXPLANE[6];
	
	return true;
}
/**
*
* CCamera class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Time elapsed
*
*/
void 
CCamera::Process(float _fDeltaTime)
{
	m_vecPosition += m_vecVelocity * _fDeltaTime;
	m_vecVelocity *= 1.0f - (5.0f *  _fDeltaTime);
	m_vecLook += m_vecTargetLook * m_fRotationSpeed * _fDeltaTime;
	m_vecRotationSpeed = (m_vecTargetLook - m_vecLook);
	if(m_vecPosition.y < 0.1f)
	{
		m_vecPosition.y = 0.1f;
	}

	//Populate view matrix
	D3DXVec3Normalize(&m_vecLook, &m_vecLook);
	D3DXVec3Normalize(&m_vecRight, &m_vecRight);
	D3DXVec3Normalize(&m_vecUp, &m_vecUp);
	
	//D3DXVec3Cross(&m_vecRight, &m_vecLook, &m_vecUp);
	D3DXMatrixLookAtLH(&m_matView, &m_vecPosition, &(m_vecPosition + m_vecLook), &m_vecUp);

	D3DXMatrixInverse(&m_matWorld, 0, &m_matView);

	CalculateViewFrustum();
}
/**
*
* CCamera class ProcessInput
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pKeys Pointer to input struct
* @param _vecMouseDirection Mouse movement direction
* @param _fDT Time elapsed
*
*/
bool 
CCamera::ProcessInput(TInputStruct* _pKeys, D3DXVECTOR2& _vecMouseDirection, bool _bScrollCamera, float _fDT)
{
	m_bMouseRayCalculatedThisFrame = false;
	
	m_pPlayerInput = _pKeys;
	m_vecMouseDirection *= 0.0f;
	float fCurrentMovementSpeed = m_fMovementSpeed;
	if (_pKeys->bShift.bPressed)
	{
		fCurrentMovementSpeed = 200.0f;
	}
	if (_pKeys->bRightMouseClick.bPressed)
	{
		//Set rotation origin
		if (_pKeys->bRightMouseClick.bPreviousState == false)
		{
			m_vecCameraRotationOrigin = _pKeys->vecMouse;
		}
		D3DXVECTOR2 vecSensitivity = D3DXVECTOR2(0.1f, 0.1f);
		D3DXVECTOR2 vecMouseOffset = _pKeys->vecMouse - m_vecCameraRotationOrigin;
		m_vecTargetLook += m_vecRight * (-vecMouseOffset.x * vecSensitivity.x) * _fDT;
		m_vecTargetLook += m_vecUp * (vecMouseOffset.y * vecSensitivity.y) * _fDT;
	}
	//Move according to mouse coordinates
	float fScreenZones = 0.1f;
	if (_bScrollCamera)
	{
		if (_pKeys->vecMouse.x > ((WINDOW_WIDTH * 0.5f) - (WINDOW_WIDTH * fScreenZones)))
		{
			m_vecVelocity -= D3DXVECTOR3(m_vecRight.x, 0.0f, m_vecRight.z) * fCurrentMovementSpeed * _fDT;
		}
		if (_pKeys->vecMouse.x < (-(WINDOW_WIDTH * 0.5f) + (WINDOW_WIDTH * fScreenZones)))
		{
			m_vecVelocity += D3DXVECTOR3(m_vecRight.x, 0.0f, m_vecRight.z) * fCurrentMovementSpeed * _fDT;
		}
		if (_pKeys->vecMouse.y >((WINDOW_HEIGHT * 0.5f) - (WINDOW_HEIGHT * fScreenZones)))
		{
			m_vecVelocity += D3DXVECTOR3(m_vecLook.x, 0.0f, m_vecLook.z) * fCurrentMovementSpeed * _fDT;
		}
		if (_pKeys->vecMouse.y < (-(WINDOW_HEIGHT * 0.5f) + (WINDOW_HEIGHT * fScreenZones)))
		{
			m_vecVelocity -= D3DXVECTOR3(m_vecLook.x, 0.0f, m_vecLook.z) * fCurrentMovementSpeed * _fDT;
		}
	}
	m_vecVelocity += D3DXVECTOR3(0.0f, _pKeys->fMouseWheel, 0.0f) * (fCurrentMovementSpeed * 10.0f) * _fDT;

	//Check for camera movement
	if (m_pPlayerInput->bW.bPressed)
	{
		m_vecVelocity += m_vecLook * fCurrentMovementSpeed * _fDT;
	}
	if (m_pPlayerInput->bS.bPressed)
	{
		m_vecVelocity -= m_vecLook * fCurrentMovementSpeed * _fDT;
	}
	if (m_pPlayerInput->bA.bPressed)
	{
		m_vecVelocity += m_vecRight * fCurrentMovementSpeed * _fDT;
	}
	if (m_pPlayerInput->bD.bPressed)
	{
		m_vecVelocity -= m_vecRight * fCurrentMovementSpeed * _fDT;
	}
	
	//Recreate vectors
	D3DXVec3Cross(&m_vecRight, &m_vecLook, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXVec3Cross(&m_vecUp, &m_vecRight, &m_vecLook);
	return true;
}
/**
*
* CCamera class Calculates and returns ray from mouse point camera
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return returns the mouse ray
*
*/
TRay&
CCamera::GetMouseRay(D3DXVECTOR2& _rMousePoint)
{
	if (m_bMouseRayCalculatedThisFrame == false)
	{
		m_bMouseRayCalculatedThisFrame = true;
		//Convert mouse coordinates to -1 to 1
		float fPointX = ((2.0f * _rMousePoint.x) / static_cast<float>(WINDOW_WIDTH));
		float fPointY = (((2.0f * _rMousePoint.y) / static_cast<float>(WINDOW_HEIGHT)));

		fPointX = fPointX / m_matProj._11;
		fPointY = fPointY / m_matProj._22;

		//Get camera view inverse
		D3DXMATRIX matViewInverse;
		D3DXMatrixInverse(&matViewInverse, NULL, &m_matView);

		//Calculate direction of the ray
		m_tMouseRay.vecDirection.x = (fPointX * matViewInverse._11) + (fPointY * matViewInverse._21) + matViewInverse._31;
		m_tMouseRay.vecDirection.y = (fPointX * matViewInverse._12) + (fPointY * matViewInverse._22) + matViewInverse._32;
		m_tMouseRay.vecDirection.z = (fPointX * matViewInverse._13) + (fPointY * matViewInverse._23) + matViewInverse._33;
		D3DXVec3Normalize(&m_tMouseRay.vecDirection, &m_tMouseRay.vecDirection);

		//Get origin of camera ray
		m_tMouseRay.vecPosition = m_vecPosition;
	}
	return m_tMouseRay;
}
/**
*
* CCamera class CreateProjectionMatrix
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fAspect Aspect ratio
*
*/
void
CCamera::CreateProjectionMatrix(float _fAspect)
{
	if(m_bUsesPerspective)
	{
		D3DXMatrixPerspectiveFovLH(&m_matProj, 0.25f * PI, _fAspect, 0.1f, 100.0f);
	}
	else
	{
		D3DXMatrixOrthoLH(&m_matProj, static_cast<float>(m_iWindowWidth), static_cast<float>(m_iWindowHeight), 1.0f, 1000.0f);
	}
}
/**
*
* CCamera class SetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _vecPosition New position vector
*
*/
void  
CCamera::SetPosition(D3DXVECTOR3& _vecPosition)
{
	m_vecPosition = _vecPosition;
}
/**
*
* CCamera class SetLook
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _vecPosition New look vector
*
*/
void 
CCamera::SetLook(D3DXVECTOR3& _vecLook)
{
	m_vecTargetLook = _vecLook;
	m_vecLook = _vecLook;
}
/**
*
* CCamera class SetTargetLook
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _vecPosition New look vector
*
*/
void 
CCamera::SetTargetLook(D3DXVECTOR3& _vecTargetLook)
{
	m_vecTargetLook = _vecTargetLook;
}
/**
*
* CCamera class Calculates the view frustum
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _vecPosition New look vector
*
*/
void 
CCamera::CalculateViewFrustum()
{
	D3DXMATRIX matViewProjection;
	D3DXMatrixMultiply(&matViewProjection, &m_matView, &m_matProj);

	m_pViewFrustum[0].a = matViewProjection._14 + matViewProjection._11;
    m_pViewFrustum[0].b = matViewProjection._24 + matViewProjection._21;
    m_pViewFrustum[0].c = matViewProjection._34 + matViewProjection._31;
    m_pViewFrustum[0].d = matViewProjection._44 + matViewProjection._41;
 
    // Right plane
    m_pViewFrustum[1].a = matViewProjection._14 - matViewProjection._11;
    m_pViewFrustum[1].b = matViewProjection._24 - matViewProjection._21;
    m_pViewFrustum[1].c = matViewProjection._34 - matViewProjection._31;
    m_pViewFrustum[1].d = matViewProjection._44 - matViewProjection._41;
 
    // Top plane
    m_pViewFrustum[2].a = matViewProjection._14 - matViewProjection._12;
    m_pViewFrustum[2].b = matViewProjection._24 - matViewProjection._22;
    m_pViewFrustum[2].c = matViewProjection._34 - matViewProjection._32;
    m_pViewFrustum[2].d = matViewProjection._44 - matViewProjection._42;
 
    // Bottom plane
    m_pViewFrustum[3].a = matViewProjection._14 + matViewProjection._12;
    m_pViewFrustum[3].b = matViewProjection._24 + matViewProjection._22;
    m_pViewFrustum[3].c = matViewProjection._34 + matViewProjection._32;
    m_pViewFrustum[3].d = matViewProjection._44 + matViewProjection._42;
 
    // Near plane
    m_pViewFrustum[4].a = matViewProjection._13;
    m_pViewFrustum[4].b = matViewProjection._23;
    m_pViewFrustum[4].c = matViewProjection._33;
    m_pViewFrustum[4].d = matViewProjection._43;
 
    // Far plane
    m_pViewFrustum[5].a = matViewProjection._14 - matViewProjection._13;
    m_pViewFrustum[5].b = matViewProjection._24 - matViewProjection._23;
    m_pViewFrustum[5].c = matViewProjection._34 - matViewProjection._33;
    m_pViewFrustum[5].d = matViewProjection._44 - matViewProjection._43;
 
    // Normalize planes
    for (int i = 0; i < 6; i++ )
    {
        D3DXPlaneNormalize(&m_pViewFrustum[i], &m_pViewFrustum[i]);
    }
}
/**
*
* CCamera class Calculates the view frustum
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _vecPosition New look vector
*
*/
bool
CCamera::IsInFrustum(D3DXVECTOR3 &_vec3Position, float _fRadius)
{
	bool bResult = true;

	for(int i = 0; i < 6; i++)
    {
		if(D3DXPlaneDotCoord(&m_pViewFrustum[i], &_vec3Position) + _fRadius < 0)
        {
            // Outside the frustum, reject it!
			bResult = false; 	
        }
    }
    return(bResult);
}
/**
*
* CCamera class GetProjection
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns projection matrix
*
*/
D3DXMATRIX& 
CCamera::GetWorldMatrix()
{
	return m_matWorld;
}
/**
*
* CCamera class GetView
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns view matrix
*
*/
D3DXMATRIX& 
CCamera::GetViewMatrix()
{
	return m_matView;
}
/**
*
* CCamera class GetProjection
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns projection matrix
*
*/
D3DXMATRIX& 
CCamera::GetProjection()
{
	return m_matProj;
}
/**
*
* CCamera class GetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns position
*
*/
D3DXVECTOR3& 
CCamera::GetPosition()
{
	return m_vecPosition;
}
/**
*
* CCamera class GetVelocity
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns velocity
*
*/
D3DXVECTOR3& 
CCamera::GetVelocity()
{
	return m_vecVelocity;
}
/**
*
* CCamera class GetRotationSpeed
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns rotation speed
*
*/
D3DXVECTOR3& 
CCamera::GetRotationSpeed()
{
	return m_vecRotationSpeed;
}
/**
*
* CCamera class GetLook
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns look vector
*
*/
D3DXVECTOR3& 
CCamera::GetLook()
{
	return m_vecLook;
}