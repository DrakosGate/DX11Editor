//
//  File Name   :   resourcemanager.h
//  Description :   Class of CResourceManager
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

// Library Includes
#include <vector>
#include <map>
#include <rapidxml_utils.hpp>

// Local Includes
#include "defines.h"

// Types
struct TTexturePoolData
{
	ID3D11ShaderResourceView* pTexture;
	std::string sName;
};

// Constants
#define MAX_TEXTURE_POOL_SIZE 20

// Prototypes
struct ID3D11Device;

class CModel;
class CAnimatedModel;
class CEntityManager;

class CResourceManager
{
//Member functions
public:
	CResourceManager();
	virtual ~CResourceManager();

	virtual void Initialise(ID3D11Device* _pDevice, CEntityManager* _pEntityManager, char* _pcResourceFilename);
	void LoadPrefabTypes(ID3D11Device* _pDevice, CEntityManager* _pEntityManager, rapidxml::xml_node<>* _pPrefabNode);

	ID3D11ShaderResourceView* CreateTextureFromData(ID3D11Device* _pDevice, unsigned char* _pcData, std::string& _sTextureString, int _iWidth, int _iHeight);

	CModel*  GetModel(std::string& _pcModelName) const;
	CAnimatedModel* GetAnimatedModel(std::string& _pcAnimatedModelName) const;
	ID3D11ShaderResourceView* GetTexture(std::string& _pcTextureName) const;
	int GetTextureID(std::string& _pcTextureName) const;
	int GetTextureID(ID3D11ShaderResourceView* _pTexture) const;
	
	virtual void SendTextureDataToShader(ID3D11DeviceContext* _pDevContext);

private:
	CResourceManager(const CResourceManager& _krInstanceToCopy);
	const CResourceManager& operator =(const CResourceManager& _krInstanceToCopy);

//Member variables
protected:
	std::vector<TTexturePoolData*> m_TexturePool;
	std::map<std::string, CModel*> m_mapModels;
	std::map<std::string, CAnimatedModel*> m_mapAnimations;

	ID3D11ShaderResourceView** m_pTextureArray;

};


#endif //__RESOURCE_MANAGER_H__