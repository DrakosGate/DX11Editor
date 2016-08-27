#pragma once

#include "mathlibrary.h"
#include "scenehierarchy.h"

//Vertex structures
struct TVertexType
{
	Math::Vector3 vecPos;
	Math::Vector2 texCoord;
	Math::Vector3 vecNormal;
	Math::Vector3 vecTangent;
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
	TPrefabOptions(std::string& _pcPrefabName, Model* _pModel, ID3D11ShaderResourceView* _pTexture, Math::Vector3& _rScale, EAIType _eAIType, bool _bIsAnimated, bool _bIsStatic)
	{
		pcPrefabName = _pcPrefabName;
		pModel = _pModel;
		pTexture = _pTexture;
		vecScale = _rScale;
		eAIType = _eAIType;
		bIsAnimated = _bIsAnimated;
		bIsStatic = _bIsStatic;
	}
	~TPrefabOptions()
	{
		vecChildren.clear();
	}
	std::string pcPrefabName;
	Model* pModel;
	ID3D11ShaderResourceView* pTexture;
	Math::Vector3 vecScale;
	EAIType eAIType;
	bool bIsAnimated;
	bool bIsStatic;
	
	std::vector<TSceneNode*> vecChildren;
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
