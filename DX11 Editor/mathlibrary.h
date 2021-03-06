#pragma once

// Includes
#include "defines.h"

#include <algorithm>
#ifdef PLANET_DX11
#	include <DirectXMath.h>
#elif defined OPENGL
#	include <glm/glm.hpp>
#endif

#define PI 3.14159265358979323846f

namespace Math
{

#ifdef PLANET_DX11
	// Type definitions
	struct Matrix
	{
		Matrix(){}
		Matrix( const DirectX::XMMATRIX& copy ){ DirectX::XMStoreFloat4x4( &data, copy ); }
		DirectX::XMFLOAT4X4 data;

		DirectX::XMMATRIX GetMatrix() const { return DirectX::XMLoadFloat4x4( &data ); };
		
		Matrix operator* ( const Matrix& other ) const { return Matrix( GetMatrix() * other.GetMatrix() ); };
	};
	struct Vector2
	{
		Vector2() : x( data.x ), y( data.y ) { x = 0.0f; y = 0.0f; }
		Vector2( const float x, const float y ) : x( data.x ), y( data.y ) { data.x = x; data.y = y; }
		Vector2( const DirectX::XMFLOAT2& copy ) : x( data.x ), y( data.y ) { data = copy; }
		DirectX::XMFLOAT2 data;
		float& X() { return data.x; }
		float& Y() { return data.y; }
		void operator= ( const Vector2& other ){ data = other.data; }
		void operator*= ( const float num ){ data.x *= num; data.y *= num; }
		Vector2 operator* ( const float num ) const { return Vector2( data.x * num, data.y * num ); }
		void operator+= ( const Vector2& other ){ data.x += other.data.x; data.y += other.data.y; }
		Vector2 operator+ ( const Vector2& other ) const { return Vector2( data.x + other.data.x, data.y + other.data.y ); }
		void operator-= ( const Vector2& other ){ data.x -= other.data.x; data.y -= other.data.y; }
		Vector2 operator- ( const Vector2& other ) const { return Vector2( data.x - other.data.x, data.y - other.data.y ); }
		
		float& x;
		float& y;
	};
	struct Vector3
	{
		Vector3() : x( data.x ), y( data.y ), z( data.z ){ x = 0.0f; y = 0.0f; z = 0.0f; }
		Vector3( const float x, const float y, const float z ) : x( data.x ), y( data.y ), z( data.z )
		{ 
			data.x = x; 
			data.y = y; 
			data.z = z; 
		}
		Vector3( DirectX::XMFLOAT3& copy ) : x( data.x ), y( data.y ), z( data.z ) { data = copy; }
		DirectX::XMFLOAT3 data;
		float& X() { return data.x; }
		float& Y() { return data.y; }
		float& Z() { return data.z; }
		void operator= ( const Vector3& other ){ data = other.data; }
		void operator*= ( const float num ){ data.x *= num; data.y *= num; data.z *= num; }
		Vector3 operator* ( const float num ) const { return Vector3( data.x * num, data.y * num, data.z * num ); }
		void operator+= ( const Vector3& other ){ data.x += other.data.x; data.y += other.data.y; data.z += other.data.z; }
		Vector3 operator+ ( const Vector3& other ) const { return Vector3( data.x + other.data.x, data.y + other.data.y, data.z + other.data.z ); }
		void operator-= ( const Vector3& other ){ data.x -= other.data.x; data.y -= other.data.y; data.z -= other.data.z; }
		Vector3 operator- ( const Vector3& other ) const { return Vector3( data.x - other.data.x, data.y - other.data.y, data.z - other.data.z ); }

		float Length()
		{ 
			DirectX::XMFLOAT3 result;
			DirectX::XMStoreFloat3( &result, DirectX::XMVector3Length( DirectX::XMLoadFloat3( &data ) ) ); 
			return result.x;
		}
		float LengthSq()
		{ 
			DirectX::XMFLOAT3 result;
			DirectX::XMStoreFloat3( &result, DirectX::XMVector3LengthSq( DirectX::XMLoadFloat3( &data ) ) ); 
			return result.x;
		}

		float& x;
		float& y;
		float& z;
	};
	struct Vector4
	{
		Vector4() : x( data.x ), y( data.y ), z( data.z ), w( data.w ) { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }
		Vector4( const float x, const float y, const float z, const float w ) : x( data.x ), y( data.y ), z( data.z ), w( data.w ) 
		{ 
			data.x = x; 
			data.y = y; 
			data.z = z; 
			data.w = w; 
		}
		Vector4( DirectX::XMFLOAT4& copy ): x( data.x ), y( data.y ), z( data.z ), w( data.w ){ data = copy; }
		DirectX::XMFLOAT4 data;
		float& X() { return data.x; }
		float& Y() { return data.y; }
		float& Z() { return data.z; }
		float& W() { return data.w; }
		void operator= ( Vector4& other ){ data = other.data; }
		void operator*= ( float num ){ data.x *= num; data.y *= num; data.z *= num; data.w *= num; }
		Vector4 operator* ( float num ){ return Vector4( data.x * num, data.y * num, data.z * num, data.w * num ); }
		void operator+= ( Vector4& other ){ data.x += other.data.x; data.y += other.data.y; data.z += other.data.z; data.w += other.data.w; }
		Vector4 operator+ ( Vector4& other ){ return Vector4( data.x + other.data.x, data.y + other.data.y, data.z + other.data.z, data.w + other.data.w ); }
		void operator-= ( Vector4& other ){ data.x -= other.data.x; data.y -= other.data.y; data.z -= other.data.z; data.w -= other.data.w; }
		Vector4 operator- ( Vector4& other ){ return Vector4( data.x - other.data.x, data.y - other.data.y, data.z - other.data.z, data.w - other.data.w ); }
		
		float& x;
		float& y;
		float& z;
		float& w;
	};

	struct Plane
	{
		Vector3 normal;
		float distance;

		void Normalise(){ DirectX::XMStoreFloat3( &normal.data, DirectX::XMPlaneNormalize( DirectX::XMLoadFloat3( &normal.data ) ) ); };
	};

	typedef Vector4 Colour;
	typedef Vector4 Quaternion;
	/*struct Quaternion
	{
		Quaternion(){}
		Quaternion( DirectX::XMFLOAT4& copy ){ data = copy; }
		DirectX::XMFLOAT4 data;
	};*/
	//struct Colour : Vector4
	//{
	//	Colour() : Vector4( 0.0f, 0.0f, 0.0f, 1.0f ) {}
	//	Colour( const float r, const float g, const float b, const float a = 1.0f )
	//		: Vector4( r, g, b, a )
	//	{
	//	
	//	}
	//};

	// Function declarations

	Matrix Translate( const Matrix& input, const Vector3& position );
	Matrix Rotate( const Matrix& input, const float angle, const Vector3& axis );
	Matrix Scale( const Matrix& input, const Vector3& scale );
	Matrix MatrixIdentity();
	Matrix MatrixLookAtLH( Vector3& eye, Vector3& target, Vector3& up );
	Matrix MatrixInverse( const float* pDeterminant, const Matrix& matrix );
	Matrix MatrixPerspectiveFovLH( const float _fov, const float _aspect, const float _nearPlane, const float _farPlane );
	Matrix MatrixOrthoLH( const float _WindowWidth, const float _WindowHeight, const float _nearPlane, const float _farPlane );
	Matrix MatrixTranspose( const Matrix& source );
	Matrix MatrixTransformation(	const Math::Vector3* ScalingOrigin, 
									const Math::Quaternion* ScalingOrientationQuaternion, 
									const Math::Vector3* Scaling, 
									const Math::Vector3* RotationOrigin, 
									const Math::Quaternion* RotationQuaternion, 
									const Math::Vector4* Translation );

	Vector2 Add( const Vector2& _lhs, const Vector2& _rhs );
	Vector3 Add( const Vector3& _lhs, const Vector3& _rhs );
	Vector4 Add( const Vector4& _lhs, const Vector4& _rhs );
	Vector2 Sub( const Vector2& _lhs, const Vector2& _rhs );
	Vector3 Sub( const Vector3& _lhs, const Vector3& _rhs );
	Vector4 Sub( const Vector4& _lhs, const Vector4& _rhs );
	float Dot( const Vector3& _lhs, const Vector3& _rhs );
	Vector3 Cross( const Vector3& _lhs, const Vector3& _rhs );
	Vector3 Normalise( const Vector3& _input );
	Vector3 Vec4To3( const Vector4& input );
	Vector4 Vec3To4( const Vector3& input );

	Quaternion QuaternionRotationYawPitchRoll( const float yaw, const float pitch, const float roll );

	float PlaneDotCoord( const Plane& plane, const Vector3& vector );

#endif

#ifdef PLANET_OPENGL
	// Type definitions
	typedef glm::mat4 Matrix;
	typedef glm::quat Quat;
	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;

	// Function declarations
	Matrix Translate( const Matrix& input, const Vector3& position );
	Matrix Rotate( const Matrix& input, const float angle, const Vector3& axis );
	Matrix Scale( const Matrix& input, const Vector3& scale );

	Vector3 Cross( const Vector3& _lhs, const Vector3& _rhs );
	Vector3 Normalise( const Vector3& _input );
	float Dot( const Vector3& _lhs, const Vector3& _rhs );
	Vector4 Vec3To4( const Vector3& input );
	Vector3 Vec4To3( const Vector4& input );
#endif

}