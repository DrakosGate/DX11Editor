//
//  File Name   :   defines.cpp
//  Description :   Defines definitions
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <D3D11.h>
#include <iostream>

// Local Includes

// This Include
#include "defines.h"

// Static Variables

// Static Function Prototypes

// Implementation
void HR( HRESULT _hResult )
{
	if( _hResult != S_OK )
	{
		assert( false );
	}
}
void HRCheck(HRESULT _hResult, wchar_t* _pcErrorMessage)
{
	if (_hResult != S_OK)
	{
		Error(_pcErrorMessage);
	}
}
void BlobCheck(HRESULT _hResult, ID3D10Blob* _pBlob)
{
	if (_hResult != S_OK)
	{
		char* pBlobErrors;
		
		pBlobErrors = reinterpret_cast<char*>(_pBlob->GetBufferPointer());
		unsigned int iBufferSize = _pBlob->GetBufferSize();

		for (unsigned int iChar = 0; iChar < iBufferSize; ++iChar)
		{
			std::cout << pBlobErrors[iChar];
		}
		ReleaseCOM(_pBlob);
	}
}
//float
//PlaneToLine( Math::Vector3& _rPlanePoint, Math::Vector3& _rPlaneNormal, TRay& _rRay)
//{
//	float fPointOfIntersection =	D3DXVec3Dot(&_rPlaneNormal, &(_rPlanePoint - _rRay.vecPosition)) / 
//									D3DXVec3Dot(&_rPlaneNormal, &(_rRay.vecDirection));
//	return fPointOfIntersection;
//}
