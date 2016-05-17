//
//  File Name   :   model.h
//  Description :   Class of Model
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

// Library Includes
#include <vector>
//#include <assimp\Importer.hpp>
//#include <assimp\scene.h>
//#include <assimp\postprocess.h>

// Local Includes
#include "Vertex.h"
#include "renderentity.h"

// Types

// Constants

// Prototypes

class Model : public RenderEntity
{
	//Member functions
public:
	Model();
	virtual ~Model();

	virtual bool Initialise();
	
	virtual void LoadSquare(ID3D11Device* _pDevice, float _fScale, Math::Vector2& _rVecTiling, Math::Colour& _rColour);
	virtual void LoadTerrain(ID3D11Device* _pDevice, int _iTerrainWidth, int _iTerrainHeight, float _fScale, Math::Vector2& _rVecTiling, Math::Colour& _rColour);
	virtual void LoadFromOBJ(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, Math::Colour& _rColour);
	virtual void LoadFromFile(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, Math::Colour& _rColour);
	//virtual void LoadAIMesh(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, Math::Colour& _rColour);
	
	virtual void CalculateBillboardMatrix(Math::Matrix& _rmatView);
	virtual void SortIndicesByDepth(ID3D11Device* _pDevice, Camera* _pActiveCamera);
	
protected:
	//void LoadAIScene(const aiScene* _pScene, char* _pcFilename);
	//void LoadAIVertexData(unsigned int _iIndex, const aiMesh* _pMesh);

private:
	Model(const Model& _krInstanceToCopy);
	const Model& operator =(const Model& _krInstanceToCopy);

	//Member variables
protected:
	bool m_bUsingCustomTexture;
	float* m_pIndexDepths;
	float m_fModelScale;

};

#endif //MODEL_H__