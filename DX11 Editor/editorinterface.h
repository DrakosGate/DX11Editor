//
//  File Name   :   menuitem.h
//  Description :   Class of PointSprite
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#pragma once

#ifndef __EDITORINTERFACE_H__
#define __EDITORINTERFACE_H__

// Library Includes
#include <vector>
#include <ShObjIdl.h>

// Local Includes
#include "pointsprite.h"

// Types
struct TButton
{
	TButton()
	{
		targetColour *= 0.0f;
	}
	TButton( TPointSpriteVertex* _pForeVertex, TPointSpriteVertex* _pBackVertex, Math::Vector3& _rPosition, Math::Vector2& _rScale, Math::Colour& _rColour, bool _bIsActive )
	{
		pForegroundVertex = _pForeVertex;
		pBackgroundVertex = _pBackVertex;
		vecPosition = _rPosition;
		vecScale = _rScale;
		colour = _rColour;
		bIsActive = _bIsActive;
		targetColour *= 0.0f;
	}
	TPointSpriteVertex* pForegroundVertex;
	TPointSpriteVertex* pBackgroundVertex;
	Math::Vector3 vecPosition;
	Math::Vector2 vecScale;
	Math::Colour colour;
	Math::Colour targetColour;
	bool bIsActive;
	std::string sName;
	std::string sOptions;
	int iForegroundTexture;
	int iBackgroundTexture;
};
struct TWindow : public TButton
{
	void OffsetWindow( Math::Vector3& _rMousePos )
	{
		for( unsigned int iButton = 0; iButton < vecButtons.size(); ++iButton )
		{
			vecButtons[ iButton ]->pForegroundVertex->pos += _rMousePos;
			vecButtons[ iButton ]->pBackgroundVertex->pos += _rMousePos;
		}
		pForegroundVertex->pos += _rMousePos;
		pBackgroundVertex->pos += _rMousePos;
	}
	void SetIsActive( bool _bIsActive, Math::Colour& _rColour )
	{
		for( unsigned int iButton = 0; iButton < vecButtons.size(); ++iButton )
		{
			vecButtons[ iButton ]->bIsActive = _bIsActive;
			vecButtons[ iButton ]->targetColour = _rColour;
			//Set the colour of all buttons in this to the default colour
			for( unsigned int iButton = 0; iButton < vecButtons.size(); ++iButton )
			{
				vecButtons[ iButton ]->targetColour = _rColour;
			}
		}
		bIsActive = _bIsActive;
		targetColour = _rColour;
	}
	std::vector<TButton*> vecButtons;
	bool bCanMove;
};
enum EWindowState
{
	WINDOWSTATE_INVALID = -1,
	WINDOWSTATE_OPEN,
	WINDOWSTATE_MOUSEOVER,
	WINDOWSTATE_CLOSED,
	WINDOWSTATE_CLOSEBUTTON,
	WINDOWSTATE_MAX
};
enum EEditorState
{
	EDITOR_INVALID = -1,
	EDITOR_IDLE,
	EDITOR_SELECTED,
	EDITOR_CLOSED,
	EDITOR_MAX
};
// Constants

// Prototypes
struct ID3D11DeviceContext;

class Level;
class Shader;
class EntityManager;
class ResourceManager;
class EditorInterface;
class EntityManager;
class CDialogEvent;

class EditorInterface : public PointSprite
{
	//Member functions
public:
	EditorInterface();
	virtual ~EditorInterface();

	virtual bool Initialise( HWND _hWindow, Level* _pLevel );
	virtual void Process( float _fDeltaTime );
	virtual EEditorState GetEditorState() const;
	virtual std::string& GetSelectedPrefab();

	virtual void ToggleEditor( bool _bIsActive );
	virtual bool IsActive() const;

	void LoadLevel( ID3D11Device* _pDevice );
	void SaveLevel( ID3D11Device* _pDevice );
	void OnSave();
	virtual void RefreshBuffers( ID3D11Device* _pDevice );
	virtual bool HasCollided( Math::Vector2& _rPoint, TButton* _pButton );
	virtual void ProcessButtonPressed( ID3D11Device* _pDevice, TWindow* _pWindow, TButton* _pButton );

	virtual void LoadFromXML( ID3D11Device* _pDevice, ResourceManager* _pResourceManager, EntityManager* _pEntityManager, char* _pcXMLFilename );
	virtual void AddPrefabCreationButtons( EntityManager* _pEntityManager, ResourceManager* _pResourceManager, TWindow* _pWindow, TButton* _pNewPrefabButton );
	virtual TPointSpriteVertex* CreatePointSprite( ID3D11Device* _pDevice, Math::Vector3& _rPosition, Math::Vector2& _rScale, Math::Colour& _rColour, float _fRotation, int _iTextureID );

private:
	EditorInterface( const EditorInterface& _krInstanceToCopy );
	const EditorInterface& operator =( const EditorInterface& _krInstanceToCopy );

	//Member variables
protected:
	EEditorState m_eEditorState;
	std::vector<TWindow*> m_vecWindows;

	Math::Colour* m_pWindowColours;
	bool m_bHasChanged;
	bool m_bIsActive;

	TWindow* m_pDraggedWindow;
	Math::Vector3 m_vecInitialDraggedWindowOffset;

	std::string m_pcNextObjectCreated;
	bool m_bCreateObject;

	//Saving and Loading files
	HWND m_hWindow;
	Level* m_pCurrentLevel;
	IFileOpenDialog* m_pFileOpenDialog;
	IFileSaveDialog* m_pFileSaveDialog;
};

#endif //__EDITORINTERFACE_H__