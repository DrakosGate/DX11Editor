//
//  File Name   :   defines.h
//  Description :   Definition of commonly used structures, enums and #defines
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

struct ID3D11ShaderResourceView;
struct ID3D10Blob;
struct TRay;

class CModel;

//Program definitions
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
//#define WINDOW_WIDTH 1280
//#define WINDOW_HEIGHT 720
#define MAX_LIGHTS 50
#define SERVER_PORT 56000

//Helpful defines / functions
#define SAFEDELETE(_object) { if(_object){ delete _object;_object = 0; } }
#define SAFEDELETEARRAY(_array){ if(_array){ delete[] _array; _array = 0; } }
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

struct TUCHARColour
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};
enum EGameState
{
	GAMESTATE_INVALID = -1,
	GAMESTATE_GAME,
	GAMESTATE_PREFABSELECTED,
	GAMESTATE_OBJECTSELECTED,
	GAMESTATE_MAX
};
enum EGameScene
{
	SCENE_INVALID = -1,
	SCENE_PERMANENTSCENE,
	SCENE_3DSCENE,
	SCENE_UI,
	SCENE_DEBUG,
	SCENE_3DANIM,
	SCENE_GRASS,
	SCENE_FONT,
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
	AI_CHICKEN,
	AI_MAX
};
enum ERenderTargetType
{
	RENDER_INVALID = -1,
	RENDER_MRT,
	RENDER_DEFERRED,
	RENDER_POST,
	RENDER_FINAL,
	RENDER_MAX
};
enum EShaderType
{
	SHADER_INVALID = -1,
	SHADER_POINTSPRITE,		//GS Shaders
	SHADER_LINERENDERER,
	SHADER_FONT,
	SHADER_GRASS,

	SHADER_OBJECT,			//Object Shaders
	SHADER_UNLITOBJECT,
	SHADER_ANIMOBJECT,

	SHADER_MRT,				//Deferred Rendering Shaders
	SHADER_DEFERRED,

	SHADER_FINALOUTPUT,		//Post processing Shaders
	SHADERPOST_PIXELATE,
	SHADERPOST_RADIALBLUR,
	SHADER_MAX
};
enum EVertexLayoutType
{
	VERTEX_INVALID = -1,
	VERTEX_STATIC,
	VERTEX_ANIMATED,
	VERTEX_POINT,
	VERTEX_FONT,
	VERTEX_MAX
};
enum ERenderState
{
	RENDERSTATE_INVALID = -1,
	RENDERSTATE_GAME,
	RENDERSTATE_EDITOR,
	RENDERSTATE_DEBUG,
	RENDERSTATE_MAX
};
enum EGrassState
{
	GRASS_INVALID = -1,
	GRASS_OFF,
	GRASS_DRAWONLY,
	GRASS_DRAWWITHCOLLISIONS,
	GRASS_MAX
};
enum EProcessingMethod
{
	PROCESSING_INVALID = -1,
	PROCESSING_SEQUENTIAL,
	PROCESSING_THREADPOOL,
	PROCESSING_OPENCL,
	PROCESSING_DISTRIBUTED,
	PROCESSING_MAX
};
//Data Structures
struct TSetupStruct
{
	TSetupStruct()
	{
		iGrassDimensions = 100;
		iLogFrameSkip = 0;
		iLogFrameDuration = 0;
		iAICount = 0;
		iAStarSearchDepth = 1;
		bDoLog = false;
		bPlaySound = true;

		eRenderState = RENDERSTATE_DEBUG;
		eAIProcessing = PROCESSING_SEQUENTIAL;
		eGrassProcessing = PROCESSING_SEQUENTIAL;
		eGrassState = GRASS_OFF;

		pcLogFilename = 0;
		pcLogDescription = 0;
		pcDefaultLevel = 0;
	}
	~TSetupStruct()
	{
		SAFEDELETEARRAY(pcLogDescription);
		SAFEDELETEARRAY(pcLogFilename);
		SAFEDELETEARRAY(pcDefaultLevel);
	}
	int iGrassDimensions;
	int iLogFrameSkip;
	int iLogFrameDuration;
	int iAICount;
	int iAStarSearchDepth;
	bool bDoLog;
	bool bPlaySound;

	ERenderState eRenderState;
	EProcessingMethod eAIProcessing;
	EProcessingMethod eGrassProcessing;
	EGrassState eGrassState;

	char* pcLogFilename;
	char* pcLogDescription;
	char* pcDefaultLevel;
};
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
struct TPrefabOptions
{
	TPrefabOptions(std::string& _pcPrefabName, CModel* _pModel, ID3D11ShaderResourceView* _pTexture, D3DXVECTOR3& _rScale, EAIType _eAIType, bool _bIsAnimated, bool _bIsStatic)
	{
		pcPrefabName = _pcPrefabName;
		pModel = _pModel;
		pTexture = _pTexture;
		vecScale = _rScale;
		eAIType = _eAIType;
		bIsAnimated = _bIsAnimated;
		bIsStatic = _bIsStatic;
	}
	std::string pcPrefabName;
	CModel* pModel;
	ID3D11ShaderResourceView* pTexture;
	D3DXVECTOR3 vecScale;
	EAIType eAIType;
	bool bIsAnimated;
	bool bIsStatic;
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
		bG.bPressed = false;
		bToggleRender.bPressed = false;
		b1.bPressed = false;
		b2.bPressed = false;
		b3.bPressed = false;
		b4.bPressed = false;
		b5.bPressed = false;
		b6.bPressed = false;
		bReset.bPressed = false;
		bShift.bPressed = false;
		bCtrl.bPressed = false;
		bTilde.bPressed = false;
		bDelete.bPressed = false;
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
		bG.RecordPreviousState();
		bToggleRender.RecordPreviousState();
		b1.RecordPreviousState();
		b2.RecordPreviousState();
		b3.RecordPreviousState();
		b4.RecordPreviousState();
		b5.RecordPreviousState();
		b6.RecordPreviousState();
		bReset.RecordPreviousState();
		bShift.RecordPreviousState();
		bCtrl.RecordPreviousState();
		bTilde.RecordPreviousState();
		bDelete.RecordPreviousState();
		bLeftMouseClick.RecordPreviousState();
		bRightMouseClick.RecordPreviousState();
		vecPreviousMouse = vecMouse;
		fMouseWheel = 0.0f;
	}
	//Keys
	TButtonState bUp;
	TButtonState bDown;
	TButtonState bLeft;
	TButtonState bRight;
	TButtonState bW;
	TButtonState bA;
	TButtonState bS;
	TButtonState bD;
	TButtonState bG;
	TButtonState bToggleRender;
	TButtonState bReset;
	TButtonState bShift;
	TButtonState bCtrl;
	TButtonState bTilde;
	TButtonState bDelete;
	//Numbers
	TButtonState b1;
	TButtonState b2;
	TButtonState b3;
	TButtonState b4;
	TButtonState b5;
	TButtonState b6;
	//Mouse
	TButtonState bLeftMouseClick;
	TButtonState bRightMouseClick;
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