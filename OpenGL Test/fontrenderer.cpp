//
//  File Name   :   fontrenderer.cpp
//  Description :   Code for class CFontRenderer
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <d3d11.h>

// Local Includes

// This Include
#include "fontrenderer.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CFontRenderer class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CFontRenderer::CFontRenderer()
: m_bHasChanged(false)
, m_pLetterCoordinates(0)
, m_iNumFontLetters(0)
, m_pFontVerts(0)
{

}

/**
*
* CFontRenderer class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CFontRenderer::~CFontRenderer()
{
	for (unsigned int iVert = 0; iVert < m_Vertices.size(); ++iVert)
	{
		delete m_Vertices[iVert];
		m_Vertices[iVert] = 0;
	}
	m_Vertices.clear();
	
	if (m_pLetterCoordinates)
	{
		delete[] m_pLetterCoordinates;
		m_pLetterCoordinates = 0;
	}
	if (m_pFontVerts)
	{
		delete[] m_pFontVerts;
		m_pFontVerts = 0;
	}
}

/**
*
* CFontRenderer class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns true
*
*/
bool 
CFontRenderer::Initialise(char* _pcFontFilename, int _iFileWidth, int _iFileHeight, D3DXVECTOR3& _rPosition, D3DXVECTOR2& _rCharacterSize)
{
	m_iNumFontLetters = _iFileWidth * _iFileHeight;
	m_pLetterCoordinates = new TLetterCoordinates[m_iNumFontLetters];
	m_vecPosition = _rPosition;
	m_vecCharacterSize = _rCharacterSize;
	
	float fWidthPercentage = 1.0f / static_cast<float>(_iFileWidth);
	float fHeightPercentage = 1.0f / static_cast<float>(_iFileHeight);
	int iCurrentLetter = 0;
	for (int iHeight = 0; iHeight < _iFileHeight; ++iHeight)
	{
		for (int iWidth = 0; iWidth < _iFileWidth; ++iWidth)
		{
			m_pLetterCoordinates[iCurrentLetter] = TLetterCoordinates(	D3DXVECTOR2(iWidth * fWidthPercentage, iHeight * fHeightPercentage),
																		D3DXVECTOR2((iWidth + 1) * fWidthPercentage, (iHeight + 1) * fHeightPercentage));
			++iCurrentLetter;
		}
	}
	return true;
}
/**
*
* CFontRenderer class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CFontRenderer::Draw(ID3D11DeviceContext* _pDevice)
{
	if (m_pVertexBuffer)
	{
		UINT stride = sizeof(TFontVertex);
		UINT offset = 0;
		_pDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		_pDevice->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		_pDevice->DrawIndexed(m_iIndexCount, 0, 0);
	}
}
/**
*
* CFontRenderer class ProcessFont Rebuilds the font buffer if needed
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CFontRenderer::ProcessFont(ID3D11Device* _pDevice)
{
	if (m_bHasChanged)
	{
		D3DXVECTOR3 vecScreenOffset(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * -0.5f, 0.0f);
		for (unsigned int iVert = 0; iVert < m_Vertices.size(); ++iVert)
		{
			delete m_Vertices[iVert];
		}
		m_Vertices.clear();
		for (unsigned int iRow = 0; iRow < m_Messages.size(); ++iRow)
		{
			for (int iLetter = 0; iLetter < m_Messages[iRow].size(); ++iLetter)
			{
				int iLetterIndex = m_Messages[iRow][iLetter] - 32;
				D3DXVECTOR3 vecCurrentLetterPos(iLetter * m_vecCharacterSize.x, iRow * -m_vecCharacterSize.y, 0.0f);
				D3DXVECTOR3 vecScreenPos(m_vecPosition.x, -m_vecPosition.y, m_vecPosition.z);
				vecCurrentLetterPos += vecScreenPos - vecScreenOffset;
				m_Vertices.push_back(new TFontVertex(vecCurrentLetterPos, m_vecCharacterSize, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_pLetterCoordinates[iLetterIndex].uvTopLeft, m_pLetterCoordinates[iLetterIndex].uvBottomRight));
			}
			//int iStringLength = strlen(_pcMessage);
			//D3DXVECTOR3 vecScreenOffset(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * -0.5f, 0.0f);
			//D3DXVECTOR3 vecStartPos(_rPos.x, -_rPos.y, _rPos.z);
			//
			//for (int iLetter = 0; iLetter < iStringLength; ++iLetter)
			//{
			//	if (_pcMessage[iLetter] != ' ')
			//	{
			//		D3DXVECTOR3 vecOffset(iLetter * _rCharacterSize.x, 0.0f, 0.0f);
			//		int iLetterIndex = static_cast<int>(_pcMessage[iLetter]) - 32;
			//		m_Letters.push_back(new TLetter(_pcMessage[iLetter], vecStartPos + vecOffset - vecScreenOffset, _rCharacterSize, m_pLetterCoordinates[iLetterIndex]));
			//
			//		m_Vertices.push_back(new TFontVertex(vecStartPos + vecOffset - vecScreenOffset, _rCharacterSize, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_pLetterCoordinates[iLetterIndex].uvTopLeft, m_pLetterCoordinates[iLetterIndex].uvBottomRight));
			//	}
			//}
		}
		//Reset the "changed" state of the font
		m_bHasChanged = false;
		if (m_pVertexBuffer)
		{
			m_pVertexBuffer->Release();
		}
		if (m_pIndexBuffer)
		{
			m_pIndexBuffer->Release();
		}
		delete[] m_pIndices;

		m_iIndexCount = m_Vertices.size();
		m_pIndices = new unsigned int[m_iIndexCount];
		for (unsigned int iIndex = 0; iIndex < m_Vertices.size(); ++iIndex)
		{
			m_pIndices[iIndex] = iIndex;
		}
		
		//Recreate buffers
		CreateVertexBuffer(_pDevice);
		CreateIndexBuffer(_pDevice);
	}
}
/**
*
* CFontRenderer class Write
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcMessage Message to write to screen
*
*/
void
CFontRenderer::Write(char* _pcMessage, int _iIndex)
{
	//Adjust message buffer to size of this index
	while(_iIndex >= m_Messages.size())
	{
		m_Messages.push_back("");
	}
	m_Messages[_iIndex] = _pcMessage;
	m_bHasChanged = true;
}
/**
*
* CFontRenderer class CreateModelBuffers
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CFontRenderer::CreateVertexBuffer(ID3D11Device* _pDevice)
{
	delete[] m_pFontVerts;
	m_iVertexCount = m_Vertices.size();
	m_pFontVerts = new TFontVertex[m_iVertexCount];
	for (int iLetter = 0; iLetter < m_iVertexCount; ++iLetter)
	{
		m_pFontVerts[iLetter] = *m_Vertices[iLetter];
	}

	D3D11_BUFFER_DESC tVertexBufferDesc;
	tVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tVertexBufferDesc.ByteWidth = sizeof(TFontVertex)* m_iVertexCount;
	tVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tVertexBufferDesc.CPUAccessFlags = 0;
	tVertexBufferDesc.MiscFlags = 0;
	tVertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tVertexData;
	tVertexData.pSysMem = m_pFontVerts;
	tVertexData.SysMemPitch = 0;
	tVertexData.SysMemSlicePitch = 0;

	_pDevice->CreateBuffer(&tVertexBufferDesc, &tVertexData, &m_pVertexBuffer);
}