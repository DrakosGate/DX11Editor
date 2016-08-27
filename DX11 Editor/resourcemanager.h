//
//  File Name   :   resourcemanager.h
//  Description :   Class of ResourceManager
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
	ID3D11Texture2D* pTexture;
	ID3D11ShaderResourceView* pResourceView;
	std::string sName;
};
struct TTextureLoadData
{
	unsigned char* buffer;
	unsigned width;
	unsigned height;
};

// Constants
#define MAX_TEXTURE_POOL_SIZE 20

// Prototypes
struct ID3D11Device;
struct TPrefabDefinition;

class Model;
//class CAnimatedModel;
class EntityManager;
class SceneHierarchy;

class ResourceManager
{
	//Member functions
public:
	ResourceManager();
	virtual ~ResourceManager();

	virtual void Initialise( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, EntityManager* _pEntityManager, SceneHierarchy* _pHierarchy );
	void AddPrefabToEntityManager( EntityManager* _pEntityManager, TPrefabDefinition* _pPrefab );
	ID3D11ShaderResourceView* CreateTextureFromData( ID3D11Device* _pDevice, unsigned char* _pcData, std::string& _sTextureString, int _iWidth, int _iHeight );

	Model* GetModel( std::string& _pModelName ) const;
	//CAnimatedModel* GetAnimatedModel(std::string& _pcAnimatedModelName) const;
	ID3D11ShaderResourceView* GetTexture( std::string& _pcTextureName ) const;
	int GetTextureID( std::string& _pcTextureName ) const;
	int GetTextureID( ID3D11ShaderResourceView* _pTexture ) const;

	virtual void SendTextureDataToShader( ID3D11DeviceContext* _pDevContext );

private:
	ResourceManager( const ResourceManager& _krInstanceToCopy );
	const ResourceManager& operator =( const ResourceManager& _krInstanceToCopy );

	bool LoadTextureFromTarga( const std::string& _textureName, TTextureLoadData& out_data );
	bool LoadTextureFromPNG( const std::string& _textureName, TTextureLoadData& out_data );

	//Member variables
protected:
	std::vector<TTexturePoolData*> m_TexturePool;
	std::map<std::string, Model*> m_mapModels;
	//std::map<std::string, CAnimatedModel*> m_mapAnimations;

	ID3D11ShaderResourceView** m_pTextureArray;

};


#endif //__RESOURCE_MANAGER_H__