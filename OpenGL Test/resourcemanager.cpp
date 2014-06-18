//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   resourcemanager.cpp
//  Description :   Code for Class CResourceManager
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <D3DX11.h>
#include <rapidxml_utils.hpp>
#include <iostream>

// Local Includes
#include "model.h"
#include "animatedmodel.h"
#include "entitymanager.h"

// This Include
#include "resourcemanager.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CResourceManager class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CResourceManager::CResourceManager()
{
	
}
/**
*
* CResourceManager class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CResourceManager::~CResourceManager()
{
	for (unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture)
	{
		ReleaseCOM(m_TexturePool[iTexture]->pTexture);
		delete m_TexturePool[iTexture];
		m_TexturePool[iTexture] = 0;
	}
	m_TexturePool.clear();
	
	std::map<std::string, CModel*>::iterator modelIter;
	std::map<std::string, CAnimatedModel*>::iterator animIter = m_mapAnimations.begin();
	for (modelIter = m_mapModels.begin(); modelIter != m_mapModels.end(); ++modelIter)
	{
		delete modelIter->second;
		modelIter->second = 0;
	}
	for (animIter = m_mapAnimations.begin(); animIter != m_mapAnimations.end(); ++animIter)
	{
		delete animIter->second;
		animIter->second = 0;
	}
}
/**
*
* CResourceManager class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcResourceFilename Filename containing level resources
*
*/
void 
CResourceManager::Initialise(ID3D11Device* _pDevice, char* _pcResourceFilename)
{
	int iMaxMessageSize = 128;
	printf("==== Loading Resources from %s ====\n", _pcResourceFilename);

	//Open file containing resource information
	rapidxml::file<> xmlFile(_pcResourceFilename);
	rapidxml::xml_document<> xmlDoc;

	//Parse file string
	xmlDoc.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* pRoot = xmlDoc.first_node();
	//Find root nodes
	rapidxml::xml_node<>* pModels		= pRoot->first_node("models");
	rapidxml::xml_node<>* pAnimations	= pRoot->first_node("animations");
	rapidxml::xml_node<>* pTextures		= pRoot->first_node("textures");

	char* pcBuffer = new char[iMaxMessageSize];
	//Loop through models
	printf("\n  == LOADING MODELS\n");
	std::string sFilePrefix = pModels->first_node("fileprefix")->value();
	for(rapidxml::xml_node<>* pCurrentModel = pModels->first_node("model"); pCurrentModel; pCurrentModel = pCurrentModel->next_sibling())
	{
		std::string sModelName = pCurrentModel->first_attribute("id")->value();
		std::string sModelFilename = pCurrentModel->first_node()->value();
		//Concatenate model prefix and model filename
		sprintf_s(pcBuffer, iMaxMessageSize, "%s%s", sFilePrefix.c_str(), sModelFilename.c_str());

		CModel* pNewModel = new CModel();
		pNewModel->Initialise();
		pNewModel->LoadFromOBJ(_pDevice, 1.0f, pcBuffer, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//Add to model map
		printf("    = Model successfully loaded from %s\n", pcBuffer);
		m_mapModels[sModelName] = pNewModel;
	}
	
	//Loop through textures
	printf("\n  == LOADING TEXTURES\n");
	sFilePrefix = pTextures->first_node("fileprefix")->value();
	for(rapidxml::xml_node<>* pCurrentTexture = pTextures->first_node("texture"); pCurrentTexture; pCurrentTexture = pCurrentTexture->next_sibling())
	{
		std::string sTextureName = pCurrentTexture->first_attribute("id")->value();
		std::string sTextureFilename = pCurrentTexture->first_node()->value();
		//Concatenate model prefix and model filename
		std::string sFilename = sFilePrefix + sTextureFilename;
		sprintf_s(pcBuffer, iMaxMessageSize, "%s ", sFilename.c_str());
		
		//Load new textures from file
		ID3D11ShaderResourceView* pNewTexture;
		HRESULT hTextureResult = D3DX11CreateShaderResourceViewFromFileA( _pDevice,
																		sFilename.c_str(),
																		NULL,
																		NULL,
																		&pNewTexture,
																		NULL);
		//_pDevice->CreateTexture2D()
		
		//Add to model map
		if(hTextureResult == S_OK)
		{
			printf("    = Texture successfully loaded from %s\n", pcBuffer);
		}
		else
		{
			printf("!!!  TEXTURE LOAD FAILED: %s  !!!\n", pcBuffer);
		}		
		TTexturePoolData* pNewPoolEntry = new TTexturePoolData;
		pNewPoolEntry->pTexture = pNewTexture;
		pNewPoolEntry->sName = sTextureName;
		m_TexturePool.push_back(pNewPoolEntry);
	}
	
	//Loop through animations
	//printf("\n  == LOADING ANIMATED MODELS\n");
	//sFilePrefix = pAnimations->first_node("fileprefix")->value();
	//for(rapidxml::xml_node<>* pCurrentAnimation = pAnimations->first_node("model"); pCurrentAnimation; pCurrentAnimation = pCurrentAnimation->next_sibling())
	//{
	//	std::string sAnimationName = pCurrentAnimation->first_attribute("id")->value();
	//	std::string sAnimationFilename = pCurrentAnimation->first_node()->value();
	//	//Concatenate model prefix and model filename
	//	sprintf_s(pcBuffer, iMaxMessageSize, "%s%s", sFilePrefix.c_str(), sAnimationFilename.c_str());
	//
	//	CAnimatedModel* pNewAnim = new CAnimatedModel();
	//	pNewAnim->Initialise();
	//	pNewAnim->LoadAIMesh(_pDevice, 1.0f, pcBuffer);
	//
	//	
	//	//Add to model map 
	//	printf("    = Texture successfully loaded from %s\n", pcBuffer);
	//	m_mapAnimations[sAnimationName] = pNewAnim;
	//}

	//Clean up
	delete[] pcBuffer;
	pcBuffer = 0;
}
/**
*
* CResourceManager class Loads all of the prefab types from file
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcResourceFilename Name of file containing prefab types
*
*/
void
CResourceManager::LoadPrefabTypes(ID3D11Device* _pDevice, CEntityManager* _pEntityManager, char* _pcResourceFilename)
{
	int iMaxMessageSize = 128;
	printf("==== Loading Prefabs from %s ====\n", _pcResourceFilename);

	//Open file containing resource information
	rapidxml::file<> xmlFile(_pcResourceFilename);
	rapidxml::xml_document<> xmlDoc;

	//Parse file string
	xmlDoc.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* pRoot = xmlDoc.first_node();

	//Loop through models
	printf("\n  == LOADING PREFABS\n");
	for (rapidxml::xml_node<>* pCurrentPrefab = pRoot->first_node("prefab"); pCurrentPrefab; pCurrentPrefab = pCurrentPrefab->next_sibling())
	{
		std::string sPrefabName = pCurrentPrefab->first_attribute("id")->value();
		std::string sPrefabModel = pCurrentPrefab->first_node("model")->value();
		std::string sPrefabDiffuseTexture = pCurrentPrefab->first_node("texture")->value();

		D3DXVECTOR3 vecScale(	ReadFromString<float>(pCurrentPrefab->first_node("scale")->first_attribute("x")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("scale")->first_attribute("y")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("scale")->first_attribute("z")->value()));
		bool bIsAnimated = false;
		bool bIsStatic = false;
		if (pCurrentPrefab->first_node("animated"))
		{
			bIsAnimated = strcmp(pCurrentPrefab->first_node("animated")->value(), "true") == 0;
		}
		if (pCurrentPrefab->first_node("static"))
		{
			bIsStatic = strcmp(pCurrentPrefab->first_node("static")->value(), "true") == 0;
		}
		EAIType eAIType = AI_INVALID;
		if (pCurrentPrefab->first_node("aitype"))
		{
			if (strcmp(pCurrentPrefab->first_node("aitype")->value(), "human") == 0)
			{
				eAIType = AI_HUMAN;
			}
			if (strcmp(pCurrentPrefab->first_node("aitype")->value(), "chicken") == 0)
			{
				eAIType = AI_CHICKEN;
			}
		}

		_pEntityManager->AddPrefab(new TPrefabOptions(sPrefabName, GetModel(sPrefabModel), GetTexture(sPrefabDiffuseTexture), vecScale, eAIType, bIsAnimated, bIsStatic));
		printf("    = Prefab %s successfully loaded\n", sPrefabName.c_str());
	}
}
/**
*
* CResourceManager class Creates a texture file from an array of data
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the new texture
*
*/
ID3D11ShaderResourceView*
CResourceManager::CreateTextureFromData(ID3D11Device* _pDevice, unsigned char* _pcData, std::string& _sTextureString, int _iWidth, int _iHeight)
{
	
	ID3D11Texture2D* pNewTexture;
	TTexturePoolData* pNewPoolData = new TTexturePoolData();

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	
	textureDesc.Width = _iWidth;
	textureDesc.Height = _iHeight;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	textureDesc.MiscFlags = 0;
	textureDesc.ArraySize = 1;

	resourceData.pSysMem = _pcData;
	resourceData.SysMemPitch = _iWidth * 4 * sizeof(unsigned char);
	resourceData.SysMemSlicePitch = _iWidth * _iHeight * 4 * sizeof(unsigned char);

	HRESULT hResult = _pDevice->CreateTexture2D(&textureDesc, &resourceData, &pNewTexture);
	HRCheck(hResult, L"Could not create texture from data");
	hResult = _pDevice->CreateShaderResourceView(pNewTexture, NULL, &pNewPoolData->pTexture);
	HRCheck(hResult, L"Could not bind custom texture to shader resource view");
	pNewPoolData->sName = _sTextureString;
	m_TexturePool.push_back(pNewPoolData);
	pNewTexture->Release();
	
	return pNewPoolData->pTexture;
}
/**
*
* CResourceManager class GetModel
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcModelName Name of model
* @return Returns Model specified
*
*/
CModel* 
CResourceManager::GetModel(std::string& _pcModelName) const
{
	return (m_mapModels.find(_pcModelName)->second);
}
/**
*
* CResourceManager class GetAnimatedModel
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcAnimatedModelName Name of model
* @return Returns Model specified
*
*/
CAnimatedModel* 
CResourceManager::GetAnimatedModel(std::string& _pcAnimatedModelName) const
{
	std::string sKey = _pcAnimatedModelName;
	return (m_mapAnimations.find(sKey)->second);
}
/**
*
* CResourceManager class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcTextureName Name of texture
* @return Returns Texture specified
*
*/
ID3D11ShaderResourceView* 
CResourceManager::GetTexture(std::string& _pcTextureName) const
{
	//Loop through texture vector and return texture matching this name
	ID3D11ShaderResourceView* pTexture = 0;
	for (unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture)
	{
		if (strcmp(m_TexturePool[iTexture]->sName.c_str(), _pcTextureName.c_str()) == 0)
		{
			pTexture = m_TexturePool[iTexture]->pTexture;
			break;
		}
	}
	if (pTexture == 0)
	{
		Error(L"Could not find texture in pool!!");
	}
	return (pTexture);
}
/**
*
* CResourceManager class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcTextureName Name of texture
* @return Returns Index of the texture specified
*
*/
int
CResourceManager::GetTextureID(std::string& _pcTextureName) const
{
	//Loop through texture vector and return texture matching this name
	int iTextureID = -1;
	for (unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture)
	{
		if (strcmp(m_TexturePool[iTexture]->sName.c_str(), _pcTextureName.c_str()) == 0)
		{
			iTextureID = iTexture;
			break;
		}
	}
	if (iTextureID == -1)
	{
		Error(L"Could not find texture in pool!!");
	}
	return (iTextureID);
}
/**
*
* CResourceManager class GetTextureID
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pTexture Pointer to the texture
* @return Returns Index of the texture specified
*
*/
int
CResourceManager::GetTextureID(ID3D11ShaderResourceView* _pTexture) const
{
	//Loop through texture vector and return texture matching this name
	int iTextureID = -1;
	for (unsigned int iTexture = 0; iTexture < m_TexturePool.size(); ++iTexture)
	{
		if (m_TexturePool[iTexture]->pTexture == _pTexture)
		{
			iTextureID = iTexture;
			break;
		}
	}
	if (iTextureID == -1)
	{
		Error(L"Could not find texture in pool!!");
	}
	return (iTextureID);
}
void
CResourceManager::SendTextureDataToShader(ID3D11DeviceContext* _pDevContext)
{
	int iTextureCount = m_TexturePool.size();
	m_pTextureArray = new ID3D11ShaderResourceView*[iTextureCount];
	for (int iTexture = 0; iTexture < iTextureCount; ++iTexture)
	{
		m_pTextureArray[iTextureCount - 1 - iTexture] = m_TexturePool[iTextureCount - iTexture - 1]->pTexture;
	}
	_pDevContext->PSSetShaderResources(0, iTextureCount, m_pTextureArray);
	
	delete[] m_pTextureArray;
	m_pTextureArray = 0;
}