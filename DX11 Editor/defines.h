#pragma once


//Program definitions
#define PLANET_DX11
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define NOMINMAX
#define MAX_LIGHTS 50

// Lib includes
#include <windows.h>
#include <d3d11.h>
#include <cassert>
#include <string>
#include <sstream>
#include <d3dcompiler.h>

// Local includes

//Helpful defines / functions
#define SAFEDELETE(_object) { if( _object != nullptr ){ delete _object;_object = 0; } }
#define SAFEDELETEARRAY(_array){ if( _array != nullptr ){ delete[] _array; _array = 0; } }
#define ReleaseCOM(_COM) { if( _COM != nullptr ){ _COM->Release();_COM = 0; } }
#define Error(pwcErrorMessage) MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK)
#define Assert(expression, pwcErrorMessage) if( !expression ){ MessageBox(NULL, pwcErrorMessage, L"ERROR", MB_OK);  assert( expression ); }
void HR( HRESULT _hResult );
void HRCheck( HRESULT _hResult, wchar_t* _pcErrorMessage );
void BlobCheck( HRESULT _hResult, ID3D10Blob* _pBlob );

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
	//PROCESSING_OPENCL,
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
