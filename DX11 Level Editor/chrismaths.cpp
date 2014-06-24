//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   chrismaths.cpp
//  Description :   Definitions for my maths functions
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#include <cmath>
#include "chrismaths.h"

/**
*
* NVector Namespace containing Vector calculations and functions
*
* @author Christopher Howlett
*
*/
namespace NVector
{
	float 
	Dot(TVector3& _rVecA, TVector3& _rVecB)
	{ 
		return ( (_rVecA.fX * _rVecB.fX) + (_rVecA.fY * _rVecB.fY) + (_rVecA.fZ * _rVecB.fZ));
	}
	TVector3 
	Cross(TVector3& _rVecA, TVector3& _rVecB)
	{
		TVector3 vecReturn;
		vecReturn.fX = ( (_rVecA.fY * _rVecB.fZ) - (_rVecA.fZ * _rVecB.fY) );
		vecReturn.fY = -((_rVecA.fX * _rVecB.fZ) - (_rVecA.fZ * _rVecB.fX) );
		vecReturn.fZ = ( (_rVecA.fX * _rVecB.fY) - (_rVecA.fY * _rVecB.fX) );
		return vecReturn;
	}
	float 
	Magnitude(TVector3& _rVecor)
	{
		return (sqrtf((_rVecor.fX * _rVecor.fX) + (_rVecor.fY * _rVecor.fY) + (_rVecor.fZ * _rVecor.fZ)));
	}
	float 
	SqMagnitude(TVector3& _rVecor)
	{
		return ((_rVecor.fX * _rVecor.fX) + (_rVecor.fY * _rVecor.fY) + (_rVecor.fZ * _rVecor.fZ));
	}
	void 
	Normalise(TVector3& _rVector)
	{
		float fOneOverMagnitude = 1.0f / Magnitude(_rVector);
		_rVector = TVector3(_rVector.fX * fOneOverMagnitude, _rVector.fY * fOneOverMagnitude, _rVector.fZ * fOneOverMagnitude);
	}
};
/**
*
* NMatrix Namespace containing Matrix calculations and functions
*
* @author Christopher Howlett
*
*/
namespace NMatrix
{
	void 
	Identity(TMatrix& _rMat)
	{
		int iThisIndex = 0;
		for(int iHeight = 0; iHeight < 4; ++iHeight)
		{
			for(int iWidth = 0; iWidth < 4; ++iWidth)
			{
				_rMat.m[iThisIndex] = (iWidth == iHeight);
				++iThisIndex;
			}
		}
	}
	void 
	RotateYawPitchRoll(TMatrix& _rMat, float _fYaw, float _fPitch, float _fRoll)
	{
		_rMat.m[0] = cosf(_fYaw) * cosf(_fPitch);
		_rMat.m[1] = cosf(_fYaw) * sinf(_fPitch) * sinf(_fRoll) - sinf(_fYaw) * cosf(_fRoll);
		_rMat.m[2] = cosf(_fYaw) * sinf(_fPitch) * cosf(_fRoll) + sinf(_fYaw) * sinf(_fRoll);
		_rMat.m[3] = 0;
			 
		_rMat.m[4] = sinf(_fYaw) * cosf(_fPitch);
		_rMat.m[5] = sinf(_fYaw) * sinf(_fPitch) * sinf(_fRoll) + cosf(_fYaw) * cosf(_fRoll);
		_rMat.m[6] = sinf(_fYaw) * sinf(_fPitch) * cosf(_fRoll) - cosf(_fYaw) * sinf(_fRoll);
		_rMat.m[7] = 0;
			 
		_rMat.m[8] = -sin(_fPitch);
		_rMat.m[9] = cosf(_fPitch) * sinf(_fRoll);
		_rMat.m[10] = cosf(_fPitch) * cosf(_fRoll);
		_rMat.m[11] = 0;
			 
		_rMat.m[12] = 0;
		_rMat.m[13] = 0;
		_rMat.m[14] = 0;
		_rMat.m[15] = 1;
	}
	void 
	Translate(TMatrix& _rMat, TVector3& _rPosition)
	{
		Identity(_rMat);
		_rMat.m[12] = _rPosition.fX;
		_rMat.m[13] = _rPosition.fY;
		_rMat.m[14] = _rPosition.fZ;
	}
	void 
	Transformation(TMatrix& _rMat, TVector3& _rPosition, TVector3& _rScale, TVector3& _rRotation)
	{
		RotateYawPitchRoll(_rMat, _rRotation.fZ, _rRotation.fY, _rRotation.fX);
		_rMat.m[12] = _rPosition.fX;
		_rMat.m[13] = _rPosition.fY;
		_rMat.m[14] = _rPosition.fZ;
	}
	void 
	ProjectionPerspective(TMatrix& _rMat, float _fFOV, float _fAspect, float _fNear, float _fDepth)
	{
		float fHalfTanFieldofView = tan(_fFOV * 0.5f);
		_rMat.m[0]  = 1.0f / (_fAspect * fHalfTanFieldofView);
		_rMat.m[1]  = 0.0f;
		_rMat.m[2]  = 0.0f;
		_rMat.m[3]  = 0.0f;
		
		_rMat.m[4]  = 0.0f;
		_rMat.m[5]  = 1.0f / fHalfTanFieldofView;
		_rMat.m[6]  = 0.0f;
		_rMat.m[7]  = 0.0f;
		
		_rMat.m[8]  = 0.0f;
		_rMat.m[9]  = 0.0f;
		_rMat.m[10] = _fDepth / (_fDepth - _fNear);
		_rMat.m[11] = 1.0f;
		
		_rMat.m[12] = 0.0f;
		_rMat.m[13] = 0.0f;
		_rMat.m[14] = (-_fNear * _fDepth) / (_fDepth - _fNear);
		_rMat.m[15] = 0.0f;
	}
	void 
	ProjectionOrthogonal(TMatrix& _rMat, int _iWidth, int _iHeight, float _fFOV, float _fAspect, float _fNear, float _fDepth)
	{
		_rMat.m[0]  = 2.0f / static_cast<float>(_iWidth);
		_rMat.m[1]  = 0.0f;
		_rMat.m[2]  = 0.0f;
		_rMat.m[3]  = 0.0f;
		
		_rMat.m[4]  = 0.0f;
		_rMat.m[5]  = 2.0f / static_cast<float>(_iHeight);
		_rMat.m[6]  = 0.0f;
		_rMat.m[7]  = 0.0f;
		
		_rMat.m[8]  = 0.0f;
		_rMat.m[9]  = 0.0f;
		_rMat.m[10] = 1.0f / (_fDepth - _fNear);
		_rMat.m[11] = _fNear / (_fNear - _fDepth);
		
		_rMat.m[12] = 0.0f;
		_rMat.m[13] = 0.0f;
		_rMat.m[14] = 0.0f;
		_rMat.m[15] = 1.0f;
	}
	bool Inverse(TMatrix& _inMatrix, TMatrix& _outMatrix)
	{
		bool bInverseResult = true;
		float fInverse[16];
		float fDeterminant;
		
		fInverse[0] = _inMatrix.m[5]  * _inMatrix.m[10] * _inMatrix.m[15] - _inMatrix.m[5]  * _inMatrix.m[11] * _inMatrix.m[14] - 
						_inMatrix.m[9]  * _inMatrix.m[6]  * _inMatrix.m[15] + _inMatrix.m[9]  * _inMatrix.m[7]  * _inMatrix.m[14] +
						_inMatrix.m[13] * _inMatrix.m[6]  * _inMatrix.m[11] - _inMatrix.m[13] * _inMatrix.m[7]  * _inMatrix.m[10];

		fInverse[4] = -_inMatrix.m[4]  * _inMatrix.m[10] * _inMatrix.m[15] + _inMatrix.m[4]  * _inMatrix.m[11] * _inMatrix.m[14] + 
						_inMatrix.m[8]  * _inMatrix.m[6]  * _inMatrix.m[15] - _inMatrix.m[8]  * _inMatrix.m[7]  * _inMatrix.m[14] - 
						_inMatrix.m[12] * _inMatrix.m[6]  * _inMatrix.m[11] + _inMatrix.m[12] * _inMatrix.m[7]  * _inMatrix.m[10];

		fInverse[8] = _inMatrix.m[4]  * _inMatrix.m[9] * _inMatrix.m[15] - _inMatrix.m[4]  * _inMatrix.m[11] * _inMatrix.m[13] - 
						_inMatrix.m[8]  * _inMatrix.m[5] * _inMatrix.m[15] + _inMatrix.m[8]  * _inMatrix.m[7] * _inMatrix.m[13] + 
						_inMatrix.m[12] * _inMatrix.m[5] * _inMatrix.m[11] - _inMatrix.m[12] * _inMatrix.m[7] * _inMatrix.m[9];

		fInverse[12] = -_inMatrix.m[4]  * _inMatrix.m[9] * _inMatrix.m[14] + _inMatrix.m[4]  * _inMatrix.m[10] * _inMatrix.m[13] +
						_inMatrix.m[8]  * _inMatrix.m[5] * _inMatrix.m[14] - _inMatrix.m[8]  * _inMatrix.m[6] * _inMatrix.m[13] - 
						_inMatrix.m[12] * _inMatrix.m[5] * _inMatrix.m[10] + _inMatrix.m[12] * _inMatrix.m[6] * _inMatrix.m[9];

		fInverse[1] = -_inMatrix.m[1]  * _inMatrix.m[10] * _inMatrix.m[15] + _inMatrix.m[1]  * _inMatrix.m[11] * _inMatrix.m[14] + 
						_inMatrix.m[9]  * _inMatrix.m[2] * _inMatrix.m[15] - _inMatrix.m[9]  * _inMatrix.m[3] * _inMatrix.m[14] - 
						_inMatrix.m[13] * _inMatrix.m[2] * _inMatrix.m[11] + _inMatrix.m[13] * _inMatrix.m[3] * _inMatrix.m[10];

		fInverse[5] = _inMatrix.m[0]  * _inMatrix.m[10] * _inMatrix.m[15] - _inMatrix.m[0]  * _inMatrix.m[11] * _inMatrix.m[14] - 
						_inMatrix.m[8]  * _inMatrix.m[2] * _inMatrix.m[15] + _inMatrix.m[8]  * _inMatrix.m[3] * _inMatrix.m[14] + 
						_inMatrix.m[12] * _inMatrix.m[2] * _inMatrix.m[11] - _inMatrix.m[12] * _inMatrix.m[3] * _inMatrix.m[10];

		fInverse[9] = -_inMatrix.m[0]  * _inMatrix.m[9] * _inMatrix.m[15] + _inMatrix.m[0]  * _inMatrix.m[11] * _inMatrix.m[13] + 
						_inMatrix.m[8]  * _inMatrix.m[1] * _inMatrix.m[15] - _inMatrix.m[8]  * _inMatrix.m[3] * _inMatrix.m[13] - 
						_inMatrix.m[12] * _inMatrix.m[1] * _inMatrix.m[11] + _inMatrix.m[12] * _inMatrix.m[3] * _inMatrix.m[9];

		fInverse[13] = _inMatrix.m[0]  * _inMatrix.m[9] * _inMatrix.m[14] - _inMatrix.m[0]  * _inMatrix.m[10] * _inMatrix.m[13] - 
						_inMatrix.m[8]  * _inMatrix.m[1] * _inMatrix.m[14] + _inMatrix.m[8]  * _inMatrix.m[2] * _inMatrix.m[13] + 
						_inMatrix.m[12] * _inMatrix.m[1] * _inMatrix.m[10] - _inMatrix.m[12] * _inMatrix.m[2] * _inMatrix.m[9];

		fInverse[2] = _inMatrix.m[1]  * _inMatrix.m[6] * _inMatrix.m[15] - _inMatrix.m[1]  * _inMatrix.m[7] * _inMatrix.m[14] - 
						_inMatrix.m[5]  * _inMatrix.m[2] * _inMatrix.m[15] + _inMatrix.m[5]  * _inMatrix.m[3] * _inMatrix.m[14] + 
						_inMatrix.m[13] * _inMatrix.m[2] * _inMatrix.m[7] - _inMatrix.m[13] * _inMatrix.m[3] * _inMatrix.m[6];

		fInverse[6] = -_inMatrix.m[0]  * _inMatrix.m[6] * _inMatrix.m[15] + _inMatrix.m[0]  * _inMatrix.m[7] * _inMatrix.m[14] + 
						_inMatrix.m[4]  * _inMatrix.m[2] * _inMatrix.m[15] - _inMatrix.m[4]  * _inMatrix.m[3] * _inMatrix.m[14] - 
						_inMatrix.m[12] * _inMatrix.m[2] * _inMatrix.m[7] + _inMatrix.m[12] * _inMatrix.m[3] * _inMatrix.m[6];

		fInverse[10] =	_inMatrix.m[0]  * _inMatrix.m[5] * _inMatrix.m[15] - _inMatrix.m[0]  * _inMatrix.m[7] * _inMatrix.m[13] - 
						_inMatrix.m[4]  * _inMatrix.m[1] * _inMatrix.m[15] + _inMatrix.m[4]  * _inMatrix.m[3] * _inMatrix.m[13] + 
						_inMatrix.m[12] * _inMatrix.m[1] * _inMatrix.m[7] - _inMatrix.m[12] * _inMatrix.m[3] * _inMatrix.m[5];

		fInverse[14] = -_inMatrix.m[0]  * _inMatrix.m[5] * _inMatrix.m[14] + _inMatrix.m[0]  * _inMatrix.m[6] * _inMatrix.m[13] + 
						_inMatrix.m[4]  * _inMatrix.m[1] * _inMatrix.m[14] - _inMatrix.m[4]  * _inMatrix.m[2] * _inMatrix.m[13] - 
						_inMatrix.m[12] * _inMatrix.m[1] * _inMatrix.m[6] + _inMatrix.m[12] * _inMatrix.m[2] * _inMatrix.m[5];

		fInverse[3] = -_inMatrix.m[1] * _inMatrix.m[6] * _inMatrix.m[11] + _inMatrix.m[1] * _inMatrix.m[7] * _inMatrix.m[10] + 
						_inMatrix.m[5] * _inMatrix.m[2] * _inMatrix.m[11] - _inMatrix.m[5] * _inMatrix.m[3] * _inMatrix.m[10] - 
						_inMatrix.m[9] * _inMatrix.m[2] * _inMatrix.m[7] + _inMatrix.m[9] * _inMatrix.m[3] * _inMatrix.m[6];

		fInverse[7] = _inMatrix.m[0] * _inMatrix.m[6] * _inMatrix.m[11] - _inMatrix.m[0] * _inMatrix.m[7] * _inMatrix.m[10] - 
						_inMatrix.m[4] * _inMatrix.m[2] * _inMatrix.m[11] + _inMatrix.m[4] * _inMatrix.m[3] * _inMatrix.m[10] + 
						_inMatrix.m[8] * _inMatrix.m[2] * _inMatrix.m[7] - _inMatrix.m[8] * _inMatrix.m[3] * _inMatrix.m[6];

		fInverse[11] = -_inMatrix.m[0] * _inMatrix.m[5] * _inMatrix.m[11] + _inMatrix.m[0] * _inMatrix.m[7] * _inMatrix.m[9] + 
						_inMatrix.m[4] * _inMatrix.m[1] * _inMatrix.m[11] - _inMatrix.m[4] * _inMatrix.m[3] * _inMatrix.m[9] - 
						_inMatrix.m[8] * _inMatrix.m[1] * _inMatrix.m[7] + _inMatrix.m[8] * _inMatrix.m[3] * _inMatrix.m[5];

		fInverse[15] = _inMatrix.m[0] * _inMatrix.m[5] * _inMatrix.m[10] - _inMatrix.m[0] * _inMatrix.m[6] * _inMatrix.m[9] - 
						_inMatrix.m[4] * _inMatrix.m[1] * _inMatrix.m[10] + _inMatrix.m[4] * _inMatrix.m[2] * _inMatrix.m[9] + 
						_inMatrix.m[8] * _inMatrix.m[1] * _inMatrix.m[6] - _inMatrix.m[8] * _inMatrix.m[2] * _inMatrix.m[5];

		fDeterminant = _inMatrix.m[0] * fInverse[0] + _inMatrix.m[1] * fInverse[4] + _inMatrix.m[2] * fInverse[8] + _inMatrix.m[3] * fInverse[12];

		//Ensure determinant is NOT equal to 0
		if(fDeterminant == 0)
		{
			bInverseResult = false;
		}
		else
		{
			fDeterminant = 1.0f / fDeterminant;
			for (int iElement = 0; iElement < 16; ++iElement)
			{
				_outMatrix.m[iElement] = fInverse[iElement] * fDeterminant;
			}
		}

		return bInverseResult;
	}
};
/**
*
* NCollision Namespace containing Collision detection calculations and functions
*
* @author Christopher Howlett
*
*/
namespace NCollision
{
	float 
	PointToLine(TVector3& _rPoint, TLine& _rLine)
	{
		TVector3 vecLine = _rLine.vecPointB - _rLine.vecPointA;
		float fDistance = NVector::SqMagnitude(vecLine);
		TVector3 vecToPoint = _rPoint - _rLine.vecPointA;
		float fDot = NVector::Dot(vecLine, vecToPoint);
		fDot /= fDistance;
		return fDot;
	}
	ECollisionType 
	PlaneToPoint(TPlane& _rPlane, TVector3& _rPoint)
	{
		ECollisionType eCollisionType = COLLISION_INVALID;
		float fDistance = NVector::Dot(_rPlane.vecNormal, _rPlane.vecPointOnPlane);
		float fCollisionResult = NVector::Dot(_rPlane.vecNormal, _rPoint) - fDistance;

		if(fCollisionResult < 0.0f)
		{
			eCollisionType = BEHIND;
		}
		else if(fCollisionResult > 0.0f)
		{
			eCollisionType = INFRONT;
		}
		else
		{
			eCollisionType = ON_PLANE;
		}
		return eCollisionType;
	}
	float 
	PlaneToLine(TPlane& _rPlane, TLine& _rLine)
	{
		float fPointOfIntersection = NVector::Dot(_rPlane.vecNormal, _rPlane.vecPointOnPlane - _rLine.vecPointA) / NVector::Dot(_rPlane.vecNormal, _rLine.vecPointB - _rLine.vecPointA);
		return fPointOfIntersection;
	}
	TTriCollisionResult 
	PlaneToTriangle(TPlane& _rPlane, TTriangle& _rTri)
	{
		TTriCollisionResult tResult;
		//First Line
		TLine line(_rTri.vecPointA, _rTri.vecPointB);
		tResult.fLineA = NCollision::PlaneToLine(_rPlane, line);
		//Second Line
		line.vecPointA = _rTri.vecPointB;
		line.vecPointB = _rTri.vecPointC;
		tResult.fLineB = NCollision::PlaneToLine(_rPlane, line);
		//Third Line
		line.vecPointA = _rTri.vecPointC;
		line.vecPointB = _rTri.vecPointA;
		tResult.fLineC = NCollision::PlaneToLine(_rPlane, line);

		//Collision Result
		tResult.bHasCollided =	(tResult.fLineA > 0.0f && tResult.fLineA < 1.0f) ||
								(tResult.fLineB > 0.0f && tResult.fLineB < 1.0f) ||
								(tResult.fLineC > 0.0f && tResult.fLineC < 1.0f) ;
		return tResult;
	}
};
/**
*
* TPlane class calculations and functions
*
* @author Christopher Howlett
*
*/
TPlane::TPlane()
{
	fD = 0.0f;
	vecNormal *= 0.0f;
}
TPlane::TPlane(TVector3& _rPointA, TVector3& _rPointB, TVector3& _rPointC)
{
	vecPointOnPlane = _rPointA;
	TVector3 vectorA = _rPointB - _rPointA;
	TVector3 vectorB = _rPointC - _rPointB;

	vecNormal = NVector::Cross(vectorA, vectorB);
	vecNormal.Normalise();

	//d = Ax + By + Cz
	fD = (vecPointOnPlane.fX * vecNormal.fX) + (vecPointOnPlane.fY * vecNormal.fY) + (vecPointOnPlane.fZ * vecNormal.fZ);
}

/**
*
* TVector2 class calculations and functions
*
* @author Christopher Howlett
*
*/
TVector2::TVector2(float _fX, float _fY)
{
	fX = _fX;
	fY = _fY;
}
/**
*
* TVector4 class calculations and functions
*
* @author Christopher Howlett
*
*/
TVector4::TVector4(float _fX, float _fY, float _fZ, float _fW)
{
	fX = _fX;
	fY = _fY;
	fZ = _fZ;
	fW = _fW;
}
/**
*
* TVector3 class calculations and functions
*
* @author Christopher Howlett
*
*/
TVector3::TVector3()
{
	fX = 0.0f;
	fY = 0.0f;
	fZ = 0.0f;
}
TVector3::TVector3(float _fX, float _fY, float _fZ)
{
	fX = _fX;
	fY = _fY;
	fZ = _fZ;
}
void 
TVector3::Normalise()
{
	float fMagnitudeDivision = 1.0f / NVector::Magnitude(*this);
	*this *= fMagnitudeDivision;
}
TVector3 
TVector3::operator* (TVector3& _rOther)
{
	return TVector3(fX * _rOther.fX, fY * _rOther.fY, fZ * _rOther.fZ);
}
TVector3 
TVector3::operator* (float _fScalar)
{
	return TVector3(fX * _fScalar, fY * _fScalar, fZ * _fScalar);
}
void 
TVector3::operator*= (TVector3& _rOther)
{
	fX *= _rOther.fX;
	fY *= _rOther.fY;
	fZ *= _rOther.fZ;
}
void 
TVector3::operator*= (float _fScalar)
{
	fX *= _fScalar;
	fY *= _fScalar;
	fZ *= _fScalar;
}
bool 
TVector3::operator== (TVector3& _rOther)
{
	return ( (fX == _rOther.fX) && (fY == _rOther.fY) && (fZ == _rOther.fZ) );
}
TVector3 
TVector3::operator+ (TVector3& _rOther)
{
	return TVector3(fX + _rOther.fX, fY + _rOther.fY, fZ + _rOther.fZ);
}
void 
TVector3::operator+= (TVector3& _rOther)
{
	fX += _rOther.fX;
	fY += _rOther.fY;
	fZ += _rOther.fZ;
}
TVector3 
TVector3::operator- (TVector3& _rOther)
{
	return TVector3(fX - _rOther.fX, fY - _rOther.fY, fZ - _rOther.fZ);
}
void 
TVector3::operator-= (TVector3& _rOther)
{
	fX -= _rOther.fX;
	fY -= _rOther.fY;
	fZ -= _rOther.fZ;
}
void 
TVector3::operator/= (float _fScalar)
{
	fX /= _fScalar;
	fY /= _fScalar;
	fZ /= _fScalar;
}
/**
*
* TLine class calculations and functions
*
* @author Christopher Howlett
*
*/
TLine::TLine()
{
	vecPointA *= 0.0f;
	vecPointB *= 0.0f;
}
TLine::TLine(TVector3& _vecPointA, TVector3& _vecPointB)
{
	vecPointA = _vecPointA;
	vecPointB = _vecPointB;
}
/**
*
* TTriangle class calculations and functions
*
* @author Christopher Howlett
*
*/
TTriangle::TTriangle()
{
	vecPointA *= 0.0f;
	vecPointB *= 0.0f;
	vecPointC *= 0.0f;
}
TTriangle::TTriangle(TVector3& _rPointA, TVector3& _rPointB, TVector3& _rPointC)
{
	vecPointA = _rPointA;
	vecPointB = _rPointB;
	vecPointC = _rPointC;
}
/**
*
* TMatrix class calculations and functions
*
* @author Christopher Howlett
*
*/
int 
TMatrix::Index(int _iWidth, int _iHeight)
{
	return ((_iHeight * 4) + _iWidth);
}
TMatrix
TMatrix::operator* (TMatrix& _rOther)
{
	TMatrix tResult;
	int iCurrentIndex = 0;
    for (unsigned int i = 0 ; i < 4 ; i++) 
	{
        for (unsigned int j = 0 ; j < 4 ; j++) 
		{
            tResult.m[Index(i, j)] =	m[Index(i, 0)] * _rOther.m[Index(0, j)] +
										m[Index(i, 1)] * _rOther.m[Index(1, j)] +
										m[Index(i, 2)] * _rOther.m[Index(2, j)] +
										m[Index(i, 3)] * _rOther.m[Index(3, j)];
			++iCurrentIndex;
       }
    }
    return tResult;
}