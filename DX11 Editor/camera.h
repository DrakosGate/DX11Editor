
#pragma once

// Library Includes

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes
class CCubeInterface;

class Camera
{
	//Member functions
public:
	Camera();
	~Camera();

	bool Initialise(float _fMovementSpeed, float _fRotationSpeed, float _fOrbitSpeed, int _iWindowWidth, int _iWindowHeight, bool _bUsesPerspective);
	void Process(float _fDeltaTime);

	//TRay& GetMouseRay(Math::Vector2& _rMousePoint);
	void CreateProjectionMatrix(float _fAspect);
	void SetPosition(Math::Vector3& _vecPosition);
	void SetLook(Math::Vector3& _vecLook);
	void SetTargetLook(Math::Vector3& _vecTargetLook);
	void CalculateViewFrustum();
	bool IsInFrustum(Math::Vector3 &_vec3Position, float _fRadius);

	Math::Matrix& GetWorldMatrix();
	Math::Matrix& GetViewMatrix();
	Math::Matrix& GetProjection();
	Math::Vector3& GetPosition();
	Math::Vector3& GetVelocity();
	Math::Vector3& GetRotationSpeed();
	Math::Vector3& GetLook();

	
private:
	Camera(const Camera& _krInstanceToCopy);
	const Camera& operator =(const Camera& _krInstanceToCopy);

	//Member variables
private:
	Math::Matrix m_matWorld;
	Math::Matrix m_matView;
	Math::Matrix m_matProj;
	Math::Vector3 m_vecPosition;
	Math::Vector3 m_vecVelocity;
	Math::Vector3 m_vecTargetLook;
	Math::Vector3 m_vecRotationSpeed;
	Math::Vector3 m_vecLook;
	Math::Vector3 m_vecUp;
	Math::Vector3 m_vecRight;
	Math::Vector3 m_vecRotation;
	Math::Vector2 m_vecMouseDirection;
	Math::Quaternion m_quatRotation;

	Math::Plane* m_pViewFrustum;

	float m_fMovementSpeed;
	float m_fRotationSpeed;
	float m_fCurrentZoomHeight;

	Math::Vector2 m_veCameraRotationOrigin;
	bool m_bUsesPerspective;
	int m_iWindowWidth;
	int m_iWindowHeight;

	//TRay m_tMouseRay;
	bool m_bMouseRayCalculatedThisFrame;
};
