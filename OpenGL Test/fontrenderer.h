//
//  File Name   :   fontrenderer.h
//  Description :   Class of CFontRenderer
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __FONTRENDERER_H__
#define __FONTRENDERER_H__

// Library Includes

// Local Includes
#include "defines.h"
#include "model.h"

// Types
struct TLetterCoordinates
{
	TLetterCoordinates(){}
	TLetterCoordinates(D3DXVECTOR2& _rTopLeft, D3DXVECTOR2& _rBottomRight)
	{
		uvTopLeft = _rTopLeft;
		uvBottomRight = _rBottomRight;
	}
	D3DXVECTOR2 uvTopLeft;
	D3DXVECTOR2 uvBottomRight;
};
struct TLetter
{
	TLetter(){}
	TLetter(unsigned char _cLetter, D3DXVECTOR3& _rPos, D3DXVECTOR2& _rScale, TLetterCoordinates& _rCoordinates)
	{
		cLetter = _cLetter;
		vecPosition = _rPos;
		vecScale = _rScale;
		tCoordinates = _rCoordinates;
	}

	unsigned char cLetter;
	D3DXVECTOR3 vecPosition;
	D3DXVECTOR2 vecScale;
	TLetterCoordinates tCoordinates;
};

// Constants

// Prototypes

class CFontRenderer : public CModel
{
	//Member functions
public:
	CFontRenderer();
	virtual ~CFontRenderer();

	virtual bool Initialise(char* _pcFontFilename, int _iFileWidth, int _iFileHeight);
	void Draw(ID3D11DeviceContext* _pDevice);
	void ProcessFont(ID3D11Device* _pDevice);
	void Write(char* _pcMessage, D3DXVECTOR3& _rPos, D3DXVECTOR2& _rCharacterSize);
	
	void CreateVertexBuffer(ID3D11Device* _pDevice);

private:
	CFontRenderer(const CFontRenderer& _krInstanceToCopy);
	const CFontRenderer& operator =(const CFontRenderer& _krInstanceToCopy);

	//Member variables
protected:
	std::vector<TLetter*> m_Letters;
	std::vector<TFontVertex*> m_Vertices;

	TLetterCoordinates* m_pLetterCoordinates;
	TFontVertex* m_pFontVerts;

	int m_iNumFontLetters;
	bool m_bHasChanged;
};

#endif //__FONTRENDERER_H__