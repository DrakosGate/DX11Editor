//
//  File Name   :   defines.h
//  Description :   Definition of general structures and defines
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//
#pragma once

#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <Windows.h>
#include <string>
#include <sstream>

#include <D3DX10math.h>
#include "chrismaths.h"

struct ID3D10Blob;
struct TRay;

//Program definitions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define MAX_LIGHTS 50

//Helpful defines / functions
#define ReleaseCOM(_COM) { if(_COM){ _COM->Release();_COM = 0; } }
#define Error(pwcErrorMessage) MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK)
void HRCheck(HRESULT _hResult, wchar_t* _pcErrorMessage);
void BlobCheck(HRESULT _hResult, ID3D10Blob* _pBlob);

float PlaneToLine(D3DXVECTOR3& _rPlanePoint, D3DXVECTOR3& _rPlaneNormal, TRay& _rRay);
template<class T> T ReadFromString(std::string _sInput)
{
	std::stringstream sStream;
	sStream << _sInput;
	T result;
	sStream >> result;
	return result;
}

//Texture types
enum EGameScene
{
	SCENE_INVALID = -1,
	SCENE_3DSCENE,
	SCENE_UI,
	SCENE_DEBUG,
	SCENE_3DANIM,
	SCENE_GRASS,
	SCENE_FINAL,
	SCENE_MAX
};
enum ETileType
{
	TILE_INVALID = -1,
	TILE_HUMAN,
	TILE_TREE,
	TILE_GRASS,
	TILE_GRASSBLADES,
	TILE_CHICKEN,
	TILE_CURSOR,
	TILE_SELECTIONCURSOR,
	TILE_MAX
};
enum EModelType
{
	MODEL_INVALID = -1,
	MODEL_HUMAN,
	MODEL_TREE,
	MODEL_CURSOR,
	MODEL_MAX
};
enum EAIType
{
	AI_INVALID = -1,
	AI_HUMAN,
	AI_CREATURE,
	AI_MAX
};

//Data Structures
struct TButtonState
{
	void RecordPreviousState()
	{
		bPreviousState = bPressed;
	}
	bool bPressed;
	bool bPreviousState;
};
struct TRay
{
	TRay()
	{
		vecPosition *= 0.0f;
		vecDirection *= 0.0f;
	}
	D3DXVECTOR3 vecPosition;
	D3DXVECTOR3 vecDirection;
};
struct TInputStruct
{
	TInputStruct()
	{
		bUp.bPressed = false;
		bDown.bPressed = false;
		bLeft.bPressed = false;
		bRight.bPressed = false;
		bW.bPressed = false;
		bA.bPressed = false;
		bS.bPressed = false;
		bD.bPressed = false;
		bResetPlayer.bPressed = false;
		b1.bPressed = false;
		b2.bPressed = false;
		b3.bPressed = false;
		bReset.bPressed = false;
		bShift.bPressed = false;
		bTilde.bPressed = false;
		bLeftMouseClick.bPressed = false;
		bRightMouseClick.bPressed = false;
		vecMouse *= 0.0f;
		vecPreviousMouse *= 0.0f;
		fMouseWheel = 0.0f;
		ZeroMemory(&msg, sizeof(MSG));
	}
	void RecordPreviousInput()
	{
		bUp.RecordPreviousState();
		bDown.RecordPreviousState();
		bLeft.RecordPreviousState();
		bRight.RecordPreviousState();
		bW.RecordPreviousState();
		bA.RecordPreviousState();
		bS.RecordPreviousState();
		bD.RecordPreviousState();
		bResetPlayer.RecordPreviousState();
		b1.RecordPreviousState();
		b2.RecordPreviousState();
		b3.RecordPreviousState();
		bReset.RecordPreviousState();
		bShift.RecordPreviousState();
		bTilde.RecordPreviousState();
		bLeftMouseClick.RecordPreviousState();
		bRightMouseClick.RecordPreviousState();
		vecPreviousMouse = vecMouse;
		fMouseWheel = 0.0f;
	}
	TButtonState bUp;
	TButtonState bDown;
	TButtonState bLeft;
	TButtonState bRight;
	TButtonState bW;
	TButtonState bA;
	TButtonState bS;
	TButtonState bD;
	TButtonState bResetPlayer;
	TButtonState b1;
	TButtonState b2;
	TButtonState b3;
	TButtonState bLeftMouseClick;
	TButtonState bRightMouseClick;
	TButtonState bReset;
	TButtonState bShift;
	TButtonState bTilde;
	float fMouseWheel;
	D3DXVECTOR2 vecMouse;
	D3DXVECTOR2 vecPreviousMouse;
	MSG msg;
	TRay m_tMouseRay;
};
struct TClothParticle
{
	void Initialise(TVector3& _rPosition)
	{
		vecPosition = _rPosition;
		vecPreviousPosition = _rPosition;
		vecStartPosition = _rPosition;
		bIsFixed = false;
	}
	void ResetParticle()
	{
		vecPosition = vecStartPosition;
		vecPreviousPosition = vecStartPosition;
	}
	void SetFixed(bool _bIsFixed)
	{
		bIsFixed = _bIsFixed;
	}
	TVector3 vecPosition;
	TVector3 vecPreviousPosition;
	TVector3 vecStartPosition;
	bool bIsFixed;
};

#endif //DEFINES_H__