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
#include <D3DX10.h>
#include <vector>
#include <map>

// Local Includes

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

class CResourceManager
{
//Member functions
public:
	CResourceManager();
	virtual ~CResourceManager();

	virtual void Initialise(ID3D11Device* _pDevice, char* _pcResourceFilename);
	CModel*  GetModel(char* _pcModelName) const;
	CAnimatedModel* GetAnimatedModel(char* _pcAnimatedModelName) const;
	ID3D11ShaderResourceView* GetTexture(char* _pcTextureName) const;
	int GetTextureID(char* _pcTextureName) const;
	
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
	ID3D11Texture2D* pTexture;
};


#endif //__RESOURCE_MANAGER_H__