#pragma once


//Program definitions
#define PLANET_DX11
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define NOMINMAX
#define MAX_LIGHTS 32

// Lib includes
#include <windows.h>
#include <cassert>
#include <string>
#include <sstream>

// Local includes
#include "mathlibrary.h"

//Helpful defines / functions
#define SAFEDELETE(_object) { if(_object){ delete _object;_object = 0; } }
#define SAFEDELETEARRAY(_array){ if(_array){ delete[] _array; _array = 0; } }
#define ReleaseCOM(_COM) { if(_COM){ _COM->Release();_COM = 0; } }
#define Error(pwcErrorMessage) MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK)
#define Assert(expression, pwcErrorMessage) if( !expression ){ MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK);  assert( expression ); }
void HR( HRESULT _hResult );
void HRCheck( HRESULT _hResult, wchar_t* _pcErrorMessage );

class Model;
class Texture;

template<class T> T ReadFromString( std::string _sInput )
{
	std::stringstream sStream;
	sStream << _sInput;
	T result;
	sStream >> result;
	return result;
}

//Vertex structures
//struct TVertexType
//{
//	Math::Vector3 vecPos;
//	Math::Vector2 texCoord;
//	Math::Vector3 vecNormal;
//	Math::Vector3 vecTangent;
//};


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

enum EModelType
{
	MODEL_INVALID = -1,
	MODEL_HUMAN,
	MODEL_TREE,
	MODEL_CURSOR,
	MODEL_MAX
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

//Texture types
enum ETextureType
{
	TEXTURE_INVALID = -1,
	TEXTURE_STONE,
	TEXTURE_STONENORMAL,
	TEXTURE_HUMAN,
	TEXTURE_TREE,
	TEXTURE_GRASS, 
	TEXTURE_SQUARES,
	TEXTURE_FLOORTILE,
	TEXTURE_FLOORNORMAL,
	TEXTURE_SHIP,
	TEXTURE_SHIPNORMAL,
	TEXTURE_MAX
};

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


struct TPrefabOptions
{
	//TPrefabOptions(std::string& _pcPrefabName, Model* _pModel, ID3D11ShaderResourceView* _pTexture, Math::Vector3& _rScale, EAIType _eAIType, bool _bIsAnimated, bool _bIsStatic)
	//{
	//	pcPrefabName = _pcPrefabName;
	//	pModel = _pModel;
	//	pTexture = _pTexture;
	//	vecScale = _rScale;
	//	eAIType = _eAIType;
	//	bIsAnimated = _bIsAnimated;
	//	bIsStatic = _bIsStatic;
	//}
	//~TPrefabOptions()
	//{
	//	vecChildren.clear();
	//}
	//std::string pcPrefabName;
	//Model* pModel;
	//ID3D11ShaderResourceView* pTexture;
	//Math::Vector3 vecScale;
	//EAIType eAIType;
	//bool bIsAnimated;
	//bool bIsStatic;
	//
	//std::vector<TSceneNode*> vecChildren;
};

//Data Structures
//struct TRay
//{
//	TRay()
//	{
//		vecPosition *= 0.0f;
//		vecDirection *= 0.0f;
//	}
//	Math::Vector3 vecPosition;
//	Math::Vector3 vecDirection;
//};

//struct TClothParticle
//{
//	void Initialise(Math::Vector3& _rPosition)
//	{
//		vecPosition = _rPosition;
//		vecPreviousPosition = _rPosition;
//		vecStartPosition = _rPosition;
//		bIsFixed = false;
//	}
//	void ResetParticle()
//	{
//		vecPosition = vecStartPosition;
//		vecPreviousPosition = vecStartPosition;
//	}
//	void SetFixed(bool _bIsFixed)
//	{
//		bIsFixed = _bIsFixed;
//	}
//	Math::Vector3 vecPosition;
//	Math::Vector3 vecPreviousPosition;
//	Math::Vector3 vecStartPosition;
//	bool bIsFixed;
//};

