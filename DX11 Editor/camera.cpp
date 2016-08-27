// Library Includes

// Local Includes

// This Include
#include "camera.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* Camera class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
Camera::Camera()
	: m_fMovementSpeed( 0.0f )
	, m_fRotationSpeed( 0.0f )
	, m_pViewFrustum( 0 )
	, m_bUsesPerspective( true )
	, m_iWindowWidth( 0 )
	, m_iWindowHeight( 0 )
	, m_fCurrentZoomHeight( 0.0f )
{
	m_matView = Math::MatrixIdentity();
	m_matProj = Math::MatrixIdentity();
	m_matWorld = Math::MatrixIdentity();

	m_vecMouseDirection *= 0.0f;
	m_vecRotationSpeed *= 0.0f;
	m_vecRotation *= 0.0f;
	m_vecVelocity *= 0.0f;
	m_vecPosition *= 0.0f;

	m_veCameraRotationOrigin *= 0.0f;

	m_vecLook.X() = 0.0f;
	m_vecLook.Y() = 0.0f;
	m_vecLook.Z() = -1.0f;
	m_vecTargetLook = m_vecLook;
	m_vecUp.X() = 0.0f;
	m_vecUp.Y() = 1.0f;
	m_vecUp.Z() = 0.0f;
	m_vecRight.X() = 1.0f;
	m_vecRight.Y() = 0.0f;
	m_vecRight.Z() = 0.0f;

}
/**
*
* Camera class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
Camera::~Camera()
{
	if( m_pViewFrustum )
	{
		delete m_pViewFrustum;
		m_pViewFrustum = 0;
	}
}
/**
*
* Camera class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fMovementSpeed Camera MovementSpeed
* @param _fRotationSpeed camera rotation speed
* @return Returns true
*
*/
bool
Camera::Initialise( float _fMovementSpeed, float _fRotationSpeed, float _fOrbitSpeed, int _iWindowWidth, int _iWindowHeight, bool _bUsesPerspective )
{
	m_fMovementSpeed = _fMovementSpeed;
	m_fRotationSpeed = _fRotationSpeed;
	m_bUsesPerspective = _bUsesPerspective;
	m_iWindowWidth = _iWindowWidth;
	m_iWindowHeight = _iWindowHeight;

	m_pViewFrustum = new Math::Plane[ 6 ];

	return true;
}
/**
*
* Camera class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Time elapsed
*
*/
void
Camera::Process( float _fDeltaTime )
{
	m_vecPosition += m_vecVelocity * _fDeltaTime;
	m_vecVelocity -= m_vecVelocity * 5.0f * _fDeltaTime;
	m_vecLook += m_vecTargetLook * 10.0f * m_fRotationSpeed * _fDeltaTime;
	m_vecRotationSpeed = ( m_vecTargetLook - m_vecLook );
	if( m_vecPosition.Y() < 0.1f )
	{
		m_vecPosition.Y() = 0.1f;
	}

	//Populate view matrix
	m_vecLook = Math::Normalise( m_vecLook );
	m_vecRight = Math::Normalise( m_vecRight );
	m_vecUp = Math::Normalise( m_vecUp );

	//D3DXVec3Cross(&m_vecRight, &m_vecLook, &m_vecUp);
	m_matView = Math::MatrixLookAtLH( m_vecPosition, ( m_vecPosition + m_vecLook ), m_vecUp );

	m_matView = Math::MatrixInverse( nullptr, m_matView );

	CalculateViewFrustum();
}

//TRay&
//Camera::GetMouseRay(Math::Vector2& _rMousePoint)
//{
//	if (m_bMouseRayCalculatedThisFrame == false)
//	{
//		m_bMouseRayCalculatedThisFrame = true;
//		//Convert mouse coordinates to -1 to 1
//		float fPointX = ((2.0f * _rMousePoint.x) / static_cast<float>(WINDOW_WIDTH));
//		float fPointY = (((2.0f * _rMousePoint.y) / static_cast<float>(WINDOW_HEIGHT)));
//
//		fPointX = fPointX / m_matProj._11;
//		fPointY = fPointY / m_matProj._22;
//
//		//Get camera view inverse
//		Math::Matrix matViewInverse;
//		Math::MatrixInverse(&matViewInverse, NULL, &m_matView);
//
//		//Calculate direction of the ray
//		m_tMouseRay.vecDirection.x = (fPointX * matViewInverse._11) + (fPointY * matViewInverse._21) + matViewInverse._31;
//		m_tMouseRay.vecDirection.y = (fPointX * matViewInverse._12) + (fPointY * matViewInverse._22) + matViewInverse._32;
//		m_tMouseRay.vecDirection.z = (fPointX * matViewInverse._13) + (fPointY * matViewInverse._23) + matViewInverse._33;
//		D3DXVec3Normalize(&m_tMouseRay.vecDirection, &m_tMouseRay.vecDirection);
//
//		//Get origin of camera ray
//		m_tMouseRay.vecPosition = m_vecPosition;
//	}
//	return m_tMouseRay;
//}

void
Camera::CreateProjectionMatrix( float _fAspect )
{
	if( m_bUsesPerspective )
		m_matProj = Math::MatrixPerspectiveFovLH( 0.3f * PI, _fAspect, 0.1f, 1000.0f );
	else
		m_matProj = Math::MatrixOrthoLH( static_cast<float>( m_iWindowWidth ), static_cast<float>( m_iWindowHeight ), 1.0f, 1000.0f );
}

void
Camera::SetPosition( Math::Vector3& _vecPosition )
{
	m_vecPosition = _vecPosition;
}

void
Camera::SetLook( Math::Vector3& _vecLook )
{
	m_vecTargetLook = _vecLook;
	m_vecLook = _vecLook;
}

void
Camera::SetTargetLook( Math::Vector3& _vecTargetLook )
{
	m_vecTargetLook = _vecTargetLook;
}

void
Camera::CalculateViewFrustum()
{
	Math::Matrix matViewProjection;
	matViewProjection = m_matView * m_matProj;

	const auto matrixData = matViewProjection.data;

	m_pViewFrustum[ 0 ].normal.X() = matrixData._14 + matrixData._11;
	m_pViewFrustum[ 0 ].normal.Y() = matrixData._24 + matrixData._21;
	m_pViewFrustum[ 0 ].normal.Z() = matrixData._34 + matrixData._31;
	m_pViewFrustum[ 0 ].distance   = matrixData._44 + matrixData._41;

	// Right plane
	m_pViewFrustum[ 1 ].normal.X() = matrixData._14 - matrixData._11;
	m_pViewFrustum[ 1 ].normal.Y() = matrixData._24 - matrixData._21;
	m_pViewFrustum[ 1 ].normal.Z() = matrixData._34 - matrixData._31;
	m_pViewFrustum[ 1 ].distance   = matrixData._44 - matrixData._41;

	// Top plane
	m_pViewFrustum[ 2 ].normal.X() = matrixData._14 - matrixData._12;
	m_pViewFrustum[ 2 ].normal.Y() = matrixData._24 - matrixData._22;
	m_pViewFrustum[ 2 ].normal.Z() = matrixData._34 - matrixData._32;
	m_pViewFrustum[ 2 ].distance   = matrixData._44 - matrixData._42;

	// Bottom plane
	m_pViewFrustum[ 3 ].normal.X() = matrixData._14 + matrixData._12;
	m_pViewFrustum[ 3 ].normal.Y() = matrixData._24 + matrixData._22;
	m_pViewFrustum[ 3 ].normal.Z() = matrixData._34 + matrixData._32;
	m_pViewFrustum[ 3 ].distance   = matrixData._44 + matrixData._42;

	// Near plane
	m_pViewFrustum[ 4 ].normal.X() = matrixData._13;
	m_pViewFrustum[ 4 ].normal.Y() = matrixData._23;
	m_pViewFrustum[ 4 ].normal.Z() = matrixData._33;
	m_pViewFrustum[ 4 ].distance   = matrixData._43;

	// Far plane
	m_pViewFrustum[ 5 ].normal.X() = matrixData._14 - matrixData._13;
	m_pViewFrustum[ 5 ].normal.Y() = matrixData._24 - matrixData._23;
	m_pViewFrustum[ 5 ].normal.Z() = matrixData._34 - matrixData._33;
	m_pViewFrustum[ 5 ].distance   = matrixData._44 - matrixData._43;

	// Normalize planes
	for( int i = 0; i < 6; i++ )
	{
		m_pViewFrustum[ i ].Normalise();
	}
}

bool
Camera::IsInFrustum( Math::Vector3 &_vec3Position, float _fRadius )
{
	bool bResult = true;

	for( int iPlane = 0; iPlane < 6; iPlane++ )
	{
		if( Math::PlaneDotCoord( m_pViewFrustum[ iPlane ], _vec3Position ) + _fRadius < 0 )
		{
			// Outside the frustum, reject it!
			bResult = false;
		}
	}
	return( bResult );
}

Math::Matrix&
Camera::GetWorldMatrix()
{
	return m_matWorld;
}

Math::Matrix&
Camera::GetViewMatrix()
{
	return m_matView;
}

Math::Matrix&
Camera::GetProjection()
{
	return m_matProj;
}

Math::Vector3&
Camera::GetPosition()
{
	return m_vecPosition;
}

Math::Vector3&
Camera::GetVelocity()
{
	return m_vecVelocity;
}

Math::Vector3&
Camera::GetRotationSpeed()
{
	return m_vecRotationSpeed;
}

Math::Vector3&
Camera::GetLook()
{
	return m_vecLook;
}