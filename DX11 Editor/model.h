//
//  File Name   :   model.h
//  Description :   Class of CModel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __MODEL_H__
#define __MODEL_H__

// Library Includes
#include <D3DX10.h>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

// Local Includes
#include "Vertex.h"
#include "renderentity.h"

// Types

// Constants

// Prototypes

class CModel : public CRenderEntity
{
	//Member functions
public:
	CModel();
	virtual ~CModel();

	virtual bool Initialise();
	
	virtual void LoadSquare(ID3D11Device* _pDevice, float _fScale, D3DXVECTOR2& _rVecTiling, D3DXCOLOR& _rColour);
	virtual void LoadTerrain(ID3D11Device* _pDevice, int _iTerrainWidth, int _iTerrainHeight, float _fScale, D3DXVECTOR2& _rVecTiling, D3DXCOLOR& _rColour);
	virtual void LoadFromOBJ(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, D3DXCOLOR& _rColour);
	virtual void LoadFromFile(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, D3DXCOLOR& _rColour);
	virtual void LoadAIMesh(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, D3DXCOLOR& _rColour);
	
	virtual void CalculateBillboardMatrix(D3DXMATRIX& _rmatView);
	virtual void SortIndicesByDepth(ID3D11Device* _pDevice, CCamera* _pActiveCamera);
	
protected:
	void LoadAIScene(const aiScene* _pScene, char* _pcFilename);
	void LoadAIVertexData(unsigned int _iIndex, const aiMesh* _pMesh);

private:
	CModel(const CModel& _krInstanceToCopy);
	const CModel& operator =(const CModel& _krInstanceToCopy);

	//Member variables
protected:
	bool m_bUsingCustomTexture;
	float* m_pIndexDepths;
	float m_fModelScale;

};

#endif //MODEL_H__