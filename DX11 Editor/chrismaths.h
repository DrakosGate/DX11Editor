//
//  File Name   :   chrismaths.h
//  Description :   Declaration of my Maths functions
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __MATHS_H__
#define __MATHS_H__

#define MATRIX_SIZE 16
#define PI 3.14159265359f
#define TO_RADIANS PI / 180.0f

//Enumerated Constants
enum ECollisionType
{
	COLLISION_INVALID = -1,
	ON_PLANE,
	INFRONT,
	BEHIND,
	COLLISION_MAX
};

//Structures
struct TVector3
{
	//Member functions
	TVector3();
	TVector3(float _fX, float _fY, float _fZ);
	void Normalise();

	//Operators
	TVector3 operator* (TVector3& _rOther);
	TVector3 operator* (float _fScalar);
	void operator*= (TVector3& _rOther);
	void operator*= (float _fScalar);
	bool operator== (TVector3& _rOther);
	TVector3 operator+ (TVector3& _rOther);
	void operator+= (TVector3& _rOther);
	TVector3 operator- (TVector3& _rOther);
	void operator-= (TVector3& _rOther);
	void operator/= (float _fScalar);
	
	//Member variables
	float fX;
	float fY;
	float fZ;
};
struct TVector2
{
	TVector2(){};
	TVector2(float _fX, float _fY);
	float fX;
	float fY;
};
struct TVector4
{
	TVector4(){};
	TVector4(float _fX, float _fY, float _fZ, float _fW);
	float fX;
	float fY;
	float fZ;
	float fW;
};
struct TMatrix
{
	//Member functions
	int Index(int _iWidth, int _iHeight);
	TMatrix operator* (TMatrix& _rOther);

	//Member variables
	float m[MATRIX_SIZE];
};
struct TPlane
{
	//Member functions
	TPlane();
	TPlane(TVector3& _rPointA, TVector3& _rPointB, TVector3& _rPointC);

	//Member variables
	float fD;
	TVector3 vecNormal;
	TVector3 vecPointOnPlane;
};
struct TLine
{
	//Member functions
	TLine();
	TLine(TVector3& _vecPointA, TVector3& _vecPointB);
	
	//Member variables
	TVector3 vecPointA;
	TVector3 vecPointB;
};
struct TTriangle
{
	//Member functions
	TTriangle();
	TTriangle(TVector3& _rPointA, TVector3& _rPointB, TVector3& _rPointC);
	
	//Member variables
	TVector3 vecPointA;
	TVector3 vecPointB;
	TVector3 vecPointC;
};
struct TTriCollisionResult
{
	float fLineA;
	float fLineB;
	float fLineC;
	bool bHasCollided;
};

// Vector Functions
namespace NVector
{
	float Dot(TVector3& _rVecA, TVector3& _rVecB);
	TVector3 Cross(TVector3& _rVecA, TVector3& _rVecB);
	float Magnitude(TVector3& _rVector);
	float SqMagnitude(TVector3& _rVector);
	void Normalise(TVector3& _rVector);
};
namespace NMatrix
{
	void Identity(TMatrix& _rMat);
	void RotateYawPitchRoll(TMatrix& _rMat, float _fYaw, float _fPitch, float _fRoll);
	void Translate(TMatrix& _rMat, TVector3& _rPosition);
	void Transformation(TMatrix& _rMat, TVector3& _rPosition, TVector3& _rScale, TVector3& _rRotation);
	void ProjectionPerspective(TMatrix& _rMat, float _fFOV, float _fAspect, float _fNear, float _fDepth);
	void ProjectionOrthogonal(TMatrix& _rMat, int _iWidth, int _iHeight, float _fFOV, float _fAspect, float _fNear, float _fDepth);
	bool Inverse(TMatrix& _inMatrix, TMatrix& _outMatrix);
};
//Collision detection
namespace NCollision
{
	float PointToLine(TVector3& _rPoint, TLine& _rLine); 
	ECollisionType PlaneToPoint(TPlane& _rPlane, TVector3& _rPoint); 
	float PlaneToLine(TPlane& _rPlane, TLine& _rLine);
	TTriCollisionResult PlaneToTriangle(TPlane& _rPlane, TTriangle& _rTri);
};

#endif //MATHS_H__