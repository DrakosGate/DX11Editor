//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   model.cpp
//  Description :   Code for class CModel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <fstream>
#include <D3D11.h>

// Local Includes
#include "defines.h"
#include "Vertex.h"
#include "boundingbox.h"
#include "camera.h"

// This Include
#include "model.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CModel class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CModel::CModel()
:	m_bUsingCustomTexture(false)
,	m_pIndexDepths(0)
,	m_fModelScale(0.0f)
{

}

/**
*
* CModel class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CModel::~CModel()
{
	if(m_pIndexDepths)
	{
		delete[] m_pIndexDepths;
		m_pIndexDepths = 0;
	}
}

/**
*
* CModel class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @param _fScale Sprite Scale
* @param _pOffsetPosition Offset position of model
* @return Returns true
*
*/
bool 
CModel::Initialise()
{
	return true;
}
/**
*
* CModel class LoadSquare
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CModel::LoadSquare(ID3D11Device* _pDevice, float _fScale, D3DXVECTOR2& _rVecTiling, D3DXCOLOR& _rColour)
{
	m_iVertexCount = 4;

	// Create cube vertices
	D3DXVECTOR3 defaultNormal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 binormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	TVertex vertices[] =
	{
		TVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), _rColour, defaultNormal, tangent, binormal, D3DXVECTOR2(0.0f, _rVecTiling.y)),
		TVertex(D3DXVECTOR3(-0.5f, 0.5f, 0.0f), _rColour, defaultNormal, tangent, binormal, D3DXVECTOR2(0.0f, 0.0f)),
		TVertex(D3DXVECTOR3(0.5f, 0.5f, 0.0f), _rColour, defaultNormal, tangent, binormal, D3DXVECTOR2(_rVecTiling.x, 0.0f)),
		TVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f), _rColour, defaultNormal, tangent, binormal, _rVecTiling)
	};

	m_pVertices = new TVertex[m_iVertexCount];
	memcpy(m_pVertices, vertices, sizeof(TVertex)* m_iVertexCount);

	// Scale the box.
	for (int i = 0; i < m_iVertexCount; ++i)
	{
		m_pVertices[i].pos *= _fScale;
	}

	m_iIndexCount = 6;
	// Create the index buffer
	m_pIndices = new unsigned int[m_iIndexCount];
	m_pIndices[0] = 0;
	m_pIndices[1] = 1;
	m_pIndices[2] = 2;

	m_pIndices[3] = 2;
	m_pIndices[4] = 3;
	m_pIndices[5] = 0;

	CreateVertexBuffer(_pDevice);
	CreateIndexBuffer(_pDevice);
}
/**
*
* CModel class LoadTerrain
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CModel::LoadTerrain(ID3D11Device* _pDevice, int _iTerrainWidth, int _iTerrainHeight, float _fScale, D3DXVECTOR2& _rVecTiling, D3DXCOLOR& _rColour)
{
	m_iVertexCount = _iTerrainWidth * _iTerrainHeight;
	
	// Create cube vertices
	D3DXVECTOR3 defaultNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 binormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_pVertices = new TVertex[m_iVertexCount];
	int iCurrentVertex = 0;
	m_fModelScale = _fScale;
	float fHalfScale = _fScale * 0.5f;
	D3DXVECTOR3 vecPosDivision = D3DXVECTOR3(1.0f / static_cast<float>(_iTerrainWidth), 1.0f, 1.0f / static_cast<float>(_iTerrainHeight));
	D3DXVECTOR3 vecPos;
	for(int iHeight = 0; iHeight < _iTerrainHeight; ++iHeight)
	{
		for(int iWidth = 0; iWidth < _iTerrainWidth; ++iWidth)
		{
			vecPos = D3DXVECTOR3(	(static_cast<float>(iWidth) * vecPosDivision.x) - 0.5f + ((500 - rand() % 1000) * 0.00005f), 
									0.0f * vecPosDivision.y, 
									(static_cast<float>(iHeight) * vecPosDivision.z) - 0.5f + ((500 - rand() % 1000) * 0.00005f));
			m_pVertices[iCurrentVertex] = TVertex(	vecPos, 
													_rColour,
													defaultNormal, 
													tangent, 
													binormal, 
													D3DXVECTOR2((static_cast<float>(iWidth) * vecPosDivision.x) * _rVecTiling.x, (static_cast<float>(iHeight) * vecPosDivision.z) * _rVecTiling.y));
			++iCurrentVertex;
		}
	}

	// Scale the box.
	for(int i = 0; i < m_iVertexCount; ++i)
	{
		m_pVertices[i].pos *= _fScale;
	}

	m_iIndexCount = m_iVertexCount;
	// Create the index buffer
	m_pIndices = new unsigned int[m_iIndexCount];
	m_pIndexDepths = new float[m_iIndexCount];
	for(int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
	{
		m_pIndices[iIndex] = iIndex;
	}

	CreateVertexBuffer(_pDevice);
	CreateIndexBuffer(_pDevice);

	CalculateBoundingBox();
}
void
CModel::LoadFromOBJ(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, D3DXCOLOR& _rColour)
{
	m_fModelScale = _fScale;
	float fHalfScale = m_fModelScale * 0.5f;
	printf("Loading model %s\n", _pcFilename);

	std::ifstream modelFile;
	char cTextBuffer[256];

	//Temporary data
	D3DXVECTOR3* pPositions;
	D3DXVECTOR2* pTexCoords;
	D3DXVECTOR3* pNormals;
	D3DXVECTOR3* pFaces;
	int iNumPositions = 0;
	int iNumFaces = 0;
	int iNumNormals = 0;
	int iNumTexCoords = 0;

	//Open model file
	modelFile.open(_pcFilename);
	if (modelFile.is_open())
	{
		//Count vertices
		while (!modelFile.eof())
		{
			modelFile.getline(cTextBuffer, 256);
			if (cTextBuffer[0] == 'v')
			{
				//Found a vertex
				if (cTextBuffer[1] == ' ')
				{
					++iNumPositions;
				}
				//Texture coordinate data
				else if (cTextBuffer[1] == 't')
				{
					++iNumTexCoords;
				}
				//Normal data
				else if (cTextBuffer[1] == 'n')
				{
					++iNumNormals;
				}
			}
			//Face data
			else if (cTextBuffer[0] == 'f')
			{
				iNumFaces += 3;
			}

		}
		modelFile.close();
		modelFile.open(_pcFilename);
		//Create vertex and index array
		m_iVertexCount = iNumFaces;
		m_iIndexCount = iNumFaces;
		m_pVertices = new TVertex[m_iVertexCount];
		m_pIndices = new unsigned int[m_iIndexCount];

		//Create temporary containers
		pPositions = new D3DXVECTOR3[iNumPositions];
		pTexCoords = new D3DXVECTOR2[iNumTexCoords];
		pNormals = new D3DXVECTOR3[iNumNormals];
		pFaces = new D3DXVECTOR3[iNumFaces];

		//Counters
		int iCurrentPosition = 0;
		int iCurrentTex = 0;
		int iCurrentNormal = 0;
		int iCurrentFace = 0;
		char cStartLetter;
		char cInput;
		while (!modelFile.eof())
		{
			modelFile.get(cStartLetter);
			//Starts with 'v'
			if (cStartLetter == 'v')
			{
				modelFile.get(cStartLetter);
				//Position data
				if (cStartLetter == ' ')
				{
					modelFile >> pPositions[iCurrentPosition].x >> pPositions[iCurrentPosition].y >> pPositions[iCurrentPosition].z;
					//pPositions[iCurrentPosition].z *= -1.0f;
					++iCurrentPosition;
				}
				//Texture coordinate data
				if (cStartLetter == 't')
				{
					modelFile >> pTexCoords[iCurrentTex].x >> pTexCoords[iCurrentTex].y;
					pTexCoords[iCurrentTex].y *= -1.0f;
					++iCurrentTex;
				}
				//Normal data
				if (cStartLetter == 'n')
				{
					modelFile >> pNormals[iCurrentNormal].x >> pNormals[iCurrentNormal].y >> pNormals[iCurrentNormal].z;
					++iCurrentNormal;
				}
			}
			//Faces
			else if (cStartLetter == 'f')
			{
				modelFile.get(cStartLetter);
				if (cStartLetter == ' ')
				{
					//Account for backwards winding
					if (iCurrentFace < iNumFaces)
					{
						modelFile	>> pFaces[iCurrentFace].x >> cInput >> pFaces[iCurrentFace].y >> cInput >> pFaces[iCurrentFace].z
									>> pFaces[iCurrentFace + 1].x >> cInput >> pFaces[iCurrentFace + 1].y >> cInput >> pFaces[iCurrentFace + 1].z
									>> pFaces[iCurrentFace + 2].x >> cInput >> pFaces[iCurrentFace + 2].y >> cInput >> pFaces[iCurrentFace + 2].z;
						iCurrentFace += 3;
					}
				}
			}
		}
		modelFile.close();
		//Create vertices based on this index order
		for (int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
		{
			m_pIndices[iIndex] = static_cast<int>(pFaces[iIndex].x) - 1;
			m_pVertices[iIndex].pos = pPositions[m_pIndices[iIndex]] * m_fModelScale;
			m_pVertices[iIndex].texC = pTexCoords[static_cast<int>(pFaces[iIndex].y) - 1];
			m_pVertices[iIndex].normal = pNormals[static_cast<int>(pFaces[iIndex].z) - 1];
			D3DXVec3Normalize(&m_pVertices[iIndex].normal, &m_pVertices[iIndex].normal);
			D3DXVec3Cross(&m_pVertices[iIndex].tangent, &m_pVertices[iIndex].normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			m_pVertices[iIndex].colour = _rColour;
		}
		//Draw vertices in ascending order
		for (int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
		{
			m_pIndices[iIndex] = iIndex;
		}

		//Delete temporary data
		delete[] pTexCoords;
		delete[] pNormals;
		delete[] pPositions;
		delete[] pFaces;
		pFaces = 0;
		pPositions = 0;
		pTexCoords = 0;
		pNormals = 0;

		CreateVertexBuffer(_pDevice);
		CreateIndexBuffer(_pDevice);

		CalculateBoundingBox();
	}
	else
	{
		Error(L"Could not open model file");
	}
}
/**
*
* CModel class LoadFromFile
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CModel::LoadFromFile(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, D3DXCOLOR& _rColour)
{
	std::ifstream fModelFile;
	fModelFile.open(_pcFilename);
	char cText;
	char cBuffer[256];
	std::string sDummyString;

	if(fModelFile.is_open())
	{
		fModelFile.get(cText);

		//Read file until vertex count is found
		while(cText != ':')
		{
			fModelFile.get(cText);
		}		
		fModelFile >> m_iVertexCount;
		m_iIndexCount = m_iVertexCount;
		
		//Create specified number of m_pVertices
		m_pVertices = new TVertex[m_iVertexCount];
		int iCurrentVertex = 0;

		//Continue reading in all vertex data					
		//Continue searching file until next : is found
		while(cText != ':')
		{
			fModelFile.get(cText);
		}
		fModelFile.getline(cBuffer, 100);
		fModelFile.getline(cBuffer, 100);
		fModelFile.getline(cBuffer, 100);

		//pos -> normal -> texC		File: x, y, z, tu, tv, nx, ny, nz
		for(int iCurrentVertex = 0; iCurrentVertex < m_iVertexCount; ++iCurrentVertex)
		{
			fModelFile >> m_pVertices[iCurrentVertex].pos.x >> m_pVertices[iCurrentVertex].pos.y >> m_pVertices[iCurrentVertex].pos.z;
			fModelFile >> m_pVertices[iCurrentVertex].texC.x >> m_pVertices[iCurrentVertex].texC.y;
			fModelFile >> m_pVertices[iCurrentVertex].normal.x >> m_pVertices[iCurrentVertex].normal.y >> m_pVertices[iCurrentVertex].normal.z;
			D3DXVec3Cross(&m_pVertices[iCurrentVertex].tangent, &m_pVertices[iCurrentVertex].normal, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
			D3DXVec3Cross(&m_pVertices[iCurrentVertex].binormal, &m_pVertices[iCurrentVertex].normal, &m_pVertices[iCurrentVertex].tangent);
			m_pVertices[iCurrentVertex].colour = _rColour;
		}		
		
		//Create index data
		m_pIndices = new unsigned int[m_iIndexCount];
		m_pIndexDepths = new float[m_iIndexCount];
		for(int i = 0; i < m_iIndexCount; ++i)
		{
			m_pIndices[i] = i;
		}

		for(int i = 0; i < m_iVertexCount; ++i)
		{
			m_pVertices[i].pos *= _fScale;
		}
	}

	fModelFile.close();

	CreateVertexBuffer(_pDevice);
	CreateIndexBuffer(_pDevice);

	CalculateBoundingBox();
}
//=============================================================
//			ASSIMP FILE LOADING
//=============================================================
/**
*
* CModel class LoadFromFile
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CModel::LoadAIMesh(ID3D11Device* _pDevice, float _fScale, char* _pcFilename, D3DXCOLOR& _rColour)
{
	m_fModelScale = _fScale;
	Assimp::Importer aImporter;
	const aiScene* pScene = aImporter.ReadFile(_pcFilename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	if(pScene)
	{
		LoadAIScene(pScene, _pcFilename);
	}
	else
	{
		MessageBox(NULL, L"Could not load assimp scene from file", L"ERROR", MB_OK);
	}

	CreateVertexBuffer(_pDevice);
	CreateIndexBuffer(_pDevice);

	CalculateBoundingBox();
}
void 
CModel::LoadAIScene(const aiScene* _pScene, char* _pcFilename)
{
	int iNumMeshes = _pScene->mNumMeshes;

	for(int iMesh = 0; iMesh < iNumMeshes; ++iMesh)
	{
		const aiMesh* pMesh = _pScene->mMeshes[iMesh];
		LoadAIVertexData(iMesh, pMesh);
	}
}
void 
CModel::LoadAIVertexData(unsigned int _iIndex, const aiMesh* _pMesh)
{
	m_iVertexCount = _pMesh->mNumVertices;
	m_pVertices = new TVertex[m_iVertexCount];

	for(int iVertex = 0; iVertex < m_iVertexCount; ++iVertex)
	{
		const aiVector3D* pPosition = &(_pMesh->mVertices[iVertex]);
		const aiVector3D* pNormal = &(_pMesh->mNormals[iVertex]);
		const aiVector3D* pTexCoord = &(_pMesh->mTextureCoords[0][iVertex]);

		m_pVertices[iVertex].pos = D3DXVECTOR3(pPosition->x, pPosition->y, pPosition->z) * m_fModelScale;
		m_pVertices[iVertex].normal = D3DXVECTOR3(pNormal->x, pNormal->y, pNormal->z);
		m_pVertices[iVertex].texC = D3DXVECTOR2(pTexCoord->x, pTexCoord->y);
	}

	m_iIndexCount = _pMesh->mNumFaces * 3;
	m_pIndices = new unsigned int[m_iIndexCount];
	int iCurrentFace = 0;
	for(unsigned int iFace = 0; iFace < _pMesh->mNumFaces; ++iFace)
	{
		const aiFace& rFace = _pMesh->mFaces[iFace];
		m_pIndices[iCurrentFace++] = rFace.mIndices[0];
		m_pIndices[iCurrentFace++] = rFace.mIndices[1];
		m_pIndices[iCurrentFace++] = rFace.mIndices[2];
	}
}
/**
*
* CModel class CalculateBillboardMatrix : Note - Call this AFTER world matrix is calculated
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rmatView View matrix
*
*/
void 
CModel::CalculateBillboardMatrix(D3DXMATRIX& _rmatView)
{
	D3DXMatrixInverse(&m_matWorld, 0, &_rmatView);

	m_matWorld._41 = m_vecPosition.x;
	m_matWorld._42 = m_vecPosition.y;
	m_matWorld._43 = m_vecPosition.z;
}
/**
*
* CModel class SortIndicesByDepth
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pTexture new model texture
*
*/
void 
CModel::SortIndicesByDepth(ID3D11Device* _pDevice, CCamera* _pActiveCamera)
{
	m_pIndexBuffer->Release();

	//Calculate camera depths
	for(int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
	{
		m_pIndexDepths[iIndex] = D3DXVec3LengthSq(&(m_pVertices[m_pIndices[iIndex]].pos - _pActiveCamera->GetPosition()));
	}

	//Sort camera depths
	bool bIsSorted = false;
	while(bIsSorted == false)
	{
		bIsSorted = true;
		for(int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
		{
			for(int iOther = iIndex + 1; iOther < m_iIndexCount; ++iOther)
			{
				if(m_pIndexDepths[iIndex] < m_pIndexDepths[iOther])
				{
					//Swap indices
					unsigned int uiSwappedIndex = m_pIndices[iIndex];
					m_pIndices[iIndex] = m_pIndices[iOther];
					m_pIndices[iOther] = uiSwappedIndex;
					//Swap camera depths
					float fSwappedDistance = m_pIndexDepths[iIndex];
					m_pIndexDepths[iIndex] = m_pIndexDepths[iOther];
					m_pIndexDepths[iOther] = fSwappedDistance;
					bIsSorted = false;
				}
			}
		}
	}
	CreateIndexBuffer(_pDevice);
}