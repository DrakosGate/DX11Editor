#include "mathlibrary.h"


#ifdef PLANET_DX11

#elif PLANET_OPENGL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#endif

namespace Math
{

#ifdef PLANET_DX11

#pragma region Matrix
	Matrix Translate( const Matrix& input, const Vector3& position )
	{
		return Matrix( input.data * DirectX::XMMatrixTranslation( position.data.x, position.data.y, position.data.z ) );
	}

	Matrix Rotate( const Matrix& input, const float angle, const Vector3& axis )
	{
		return Matrix( input.data * DirectX::XMMatrixRotationAxis( DirectX::XMLoadFloat3( &axis.data ), angle ) );
	}

	Matrix Scale( const Matrix& input, const Vector3& scale )
	{
		return input.data * DirectX::XMMatrixScaling( scale.data.x, scale.data.y, scale.data.z );
	}

	Matrix MatrixIdentity()
	{
		return DirectX::XMMatrixIdentity();
	}

	Matrix MatrixLookAtLH( Vector3& eye, Vector3& target, Vector3& up )
	{
		return Matrix( DirectX::XMMatrixLookAtLH( DirectX::XMLoadFloat3( &eye.data ), DirectX::XMLoadFloat3( &target.data ), DirectX::XMLoadFloat3( &up.data ) ) );
	}

	Matrix MatrixInverse( const float* pDeterminant, const Matrix& matrix )
	{
		return Matrix( DirectX::XMMatrixInverse( nullptr, matrix.data ) );
	}

	Matrix MatrixPerspectiveFovLH( const float _fov, const float _aspect, const float _nearPlane, const float _farPlane )
	{
		return Matrix( DirectX::XMMatrixPerspectiveFovLH( _fov, _aspect, _nearPlane, _farPlane ) );
	}

	Matrix MatrixOrthoLH( const float _WindowWidth, const float _WindowHeight, const float _nearPlane, const float _farPlane )
	{
		return Matrix( DirectX::XMMatrixOrthographicLH( _WindowWidth, _WindowHeight, _nearPlane, _farPlane ) );
	}

	Matrix Transpose( const Matrix& source )
	{
		return Matrix( DirectX::XMMatrixTranspose( source.data ) );
	}

#pragma endregion

#pragma region Vector
	Vector2 Add( const Vector2& _lhs, const Vector2& _rhs ){ return Vector2( DirectX::XMFLOAT2( _lhs.data.x + _rhs.data.x, _lhs.data.y + _rhs.data.y ) ); }
	Vector3 Add( const Vector3& _lhs, const Vector3& _rhs ){ return Vector3( DirectX::XMFLOAT3( _lhs.data.x + _rhs.data.x, _lhs.data.y + _rhs.data.y, _lhs.data.z + _rhs.data.z ) ); }
	Vector4 Add( const Vector4& _lhs, const Vector4& _rhs ){ return Vector4( DirectX::XMFLOAT4( _lhs.data.x + _rhs.data.x, _lhs.data.y + _rhs.data.y, _lhs.data.z + _rhs.data.z, _lhs.data.w + _rhs.data.w ) ); }
	Vector2 Sub( const Vector2& _lhs, const Vector2& _rhs ){ return Vector2( DirectX::XMFLOAT2( _lhs.data.x - _rhs.data.x, _lhs.data.y - _rhs.data.y ) ); }
	Vector3 Sub( const Vector3& _lhs, const Vector3& _rhs ){ return Vector3( DirectX::XMFLOAT3( _lhs.data.x - _rhs.data.x, _lhs.data.y - _rhs.data.y, _lhs.data.z - _rhs.data.z ) ); }
	Vector4 Sub( const Vector4& _lhs, const Vector4& _rhs ){ return Vector4( DirectX::XMFLOAT4( _lhs.data.x - _rhs.data.x, _lhs.data.y - _rhs.data.y, _lhs.data.z - _rhs.data.z, _lhs.data.w - _rhs.data.w ) ); }

	float Dot( const Vector3& _lhs, const Vector3& _rhs )
	{
		Math::Vector3 result;
		DirectX::XMStoreFloat3( &result.data, DirectX::XMVector3Dot( DirectX::XMLoadFloat3( &_lhs.data ), DirectX::XMLoadFloat3( &_rhs.data ) ) );
		return result.data.x;
	}

	Math::Vector3 Cross( const Vector3& _lhs, const Vector3& _rhs )
	{
		Math::Vector3 result;
		DirectX::XMStoreFloat3( &result.data, DirectX::XMVector3Cross( DirectX::XMLoadFloat3( &_lhs.data ), DirectX::XMLoadFloat3( &_rhs.data ) ) );
		return result;
	}

	Math::Vector3 Normalise( const Vector3& _input )
	{
		Math::Vector3 result;
		DirectX::XMStoreFloat3( &result.data, DirectX::XMVector3Normalize( DirectX::XMLoadFloat3( &_input.data ) ) );
		return result;
	}

	Math::Vector3 Vec4To3( const Vector4& input )
	{
		return Vector3( DirectX::XMFLOAT3( input.data.x, input.data.y, input.data.z ) );
	}

	Math::Vector4 Vec3To4( const Vector3& input )
	{
		return Vector4( DirectX::XMFLOAT4( input.data.x, input.data.y, input.data.z, 1.0f ) );
	}
#pragma endregion

#pragma region Plane
	float PlaneDotCoord( const Plane& plane, const Vector3& vector )
	{
		DirectX::XMFLOAT3 result;
		DirectX::XMStoreFloat3( &result, DirectX::XMPlaneDotCoord( DirectX::XMLoadFloat3( &plane.normal.data ), DirectX::XMLoadFloat3( &vector.data ) ) );
		return result.x;
	}
#pragma endregion

#elif PLANET_OPENGL
#pragma region Matrix
	Matrix Translate( const Matrix& input, const Vector3& position )
	{
		return glm::translate( input, position );
	}

	Matrix Rotate( const Matrix& input, const float angle, const Vector3& axis )
	{
		return glm::rotate( input, angle, axis );
	}

	Matrix Scale( const Matrix& input, const Vector3& scale )
	{
		return glm::scale( input, scale );
	}
#pragma endregion

#pragma region Vector
	Math::Vector3 Cross( const Vector3& _lhs, const Vector3& _rhs )
	{
		return glm::cross( _lhs, _rhs );
	}

	Math::Vector3 Normalise( const Vector3& _input )
	{
		return glm::normalize( _input );
	}

	float Dot( const Vector3& _lhs, const Vector3& _rhs )
	{
		return glm::dot( _lhs, _rhs );
	}

	Math::Vector4 Vec3To4( const Vector3& input )
	{
		return Vector4( input.x, input.y, input.z, 1.0f );
	}

	Math::Vector3 Vec4To3( const Vector4& input )
	{
		return Vector3( input.x, input.y, input.z );
	}
#pragma endregion
#endif

}