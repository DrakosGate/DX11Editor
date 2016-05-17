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
	TLetterCoordinates(Math::Vector2& _rTopLeft, Math::Vector2& _rBottomRight)
	{
		uvTopLeft = _rTopLeft;
		uvBottomRight = _rBottomRight;
	}
	Math::Vector2 uvTopLeft;
	Math::Vector2 uvBottomRight;
};
struct TLetter
{
	TLetter(){}
	TLetter(unsigned char _cLetter, Math::Vector3& _rPos, Math::Vector2& _rScale, TLetterCoordinates& _rCoordinates)
	{
		cLetter = _cLetter;
		vecPosition = _rPos;
		vecScale = _rScale;
		tCoordinates = _rCoordinates;
	}

	unsigned char cLetter;
	Math::Vector3 vecPosition;
	Math::Vector2 vecScale;
	TLetterCoordinates tCoordinates;
};

// Constants

// Prototypes

class CFontRenderer : public Model
{
	//Member functions
public:
	CFontRenderer();
	virtual ~CFontRenderer();

	virtual bool Initialise(char* _pcFontFilename, int _iFileWidth, int _iFileHeight, Math::Vector3& _rPosition, Math::Vector2& _rCharacterSize, Math::Colour& _rFontColour);
	void Draw(ID3D11DeviceContext* _pDevice);
	void ProcessFont(ID3D11Device* _pDevice);
	void Write(std::string& _pcMessage, int _iIndex);
	
	void CreateVertexBuffer(ID3D11Device* _pDevice);

private:
	CFontRenderer(const CFontRenderer& _krInstanceToCopy);
	const CFontRenderer& operator =(const CFontRenderer& _krInstanceToCopy);

	//Member variables
protected:
	std::vector<std::string> m_Messages;
	std::vector<TFontVertex*> m_Vertices;

	TLetterCoordinates* m_pLetterCoordinates;
	TFontVertex* m_pFontVerts;
	Math::Vector3 m_vecPosition;
	Math::Vector2 m_vecCharacterSize;
	Math::Colour m_fontColour;

	int m_iNumFontLetters;
	bool m_bHasChanged;
};

#endif //__FONTRENDERER_H__