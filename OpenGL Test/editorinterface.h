//
//  File Name   :   menuitem.h
//  Description :   Class of CPointSprite
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
	TButton(TPointSpriteVertex* _pForeVertex, TPointSpriteVertex* _pBackVertex, D3DXVECTOR3& _rPosition, D3DXVECTOR2& _rScale, D3DXCOLOR& _rColour, bool _bIsActive)
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
	D3DXVECTOR3 vecPosition;
	D3DXVECTOR2 vecScale;
	D3DXCOLOR colour;
	D3DXCOLOR targetColour;
	bool bIsActive;
	std::string sName;
	std::string sOptions;
	int iForegroundTexture;
	int iBackgroundTexture;
};
struct TWindow : public TButton
{
	void OffsetWindow(D3DXVECTOR3& _rMousePos)
	{
		for (unsigned int iButton = 0; iButton < vecButtons.size(); ++iButton)
		{
			vecButtons[iButton]->pForegroundVertex->pos += _rMousePos;
			vecButtons[iButton]->pBackgroundVertex->pos += _rMousePos;
		}
		pForegroundVertex->pos += _rMousePos;
		pBackgroundVertex->pos += _rMousePos;
	}
	void SetIsActive(bool _bIsActive, D3DXCOLOR& _rColour)
	{
		for (unsigned int iButton = 0; iButton < vecButtons.size(); ++iButton)
		{
			vecButtons[iButton]->bIsActive = _bIsActive;
			vecButtons[iButton]->targetColour = _rColour;
			//Set the colour of all buttons in this to the default colour
			for (unsigned int iButton = 0; iButton < vecButtons.size(); ++iButton)
			{
				vecButtons[iButton]->targetColour = _rColour;
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

class CLevel;
class CShader;
class CEntityManager;
class CResourceManager;
class CEditorInterface;
class CDialogEvent;

class CEditorInterface : public CPointSprite
{
	//Member functions
public:
	CEditorInterface();
	virtual ~CEditorInterface();

	virtual bool Initialise(HWND _hWindow, CLevel* _pLevel);
	virtual void Process(float _fDeltaTime);
	virtual bool ProcessInput(TInputStruct* _pKeys, float _fDT);
	virtual EEditorState GetEditorState() const;
	virtual std::string& GetSelectedPrefab();

	virtual void ToggleEditor(bool _bIsActive);
	virtual bool IsActive() const;

	void LoadLevel();
	void SaveLevel();
	void OnSave();
	virtual void RefreshBuffers(ID3D11Device* _pDevice);
	virtual bool HasCollided(D3DXVECTOR2& _rPoint, TButton* _pButton);
	virtual void ProcessButtonPressed(TWindow* _pWindow, TButton* _pButton);
	virtual void CheckForNewObjects(ID3D11Device* _pDevice, CEntityManager* _pEntityManager, CShader* _pObjectShader, CRenderEntity* _pSpawnPos);

	virtual void LoadFromXML(ID3D11Device* _pDevice, CResourceManager* _pResourceManager, char* _pcXMLFilename);
	virtual TPointSpriteVertex* CreatePointSprite(ID3D11Device* _pDevice, D3DXVECTOR3& _rPosition, D3DXVECTOR2& _rScale, D3DXCOLOR& _rColour, float _fRotation, int _iTextureID);

	virtual void SendTextureDataToShader(ID3D11DeviceContext* _pDevContext);

private:
	CEditorInterface(const CEditorInterface& _krInstanceToCopy);
	const CEditorInterface& operator =(const CEditorInterface& _krInstanceToCopy);

	//Member variables
protected:
	EEditorState m_eEditorState;
	std::vector<TWindow*> m_vecWindows;

	D3DXCOLOR* m_pWindowColours;
	bool m_bHasChanged;
	bool m_bIsActive;

	TWindow* m_pDraggedWindow;
	D3DXVECTOR3 m_vecInitialDraggedWindowOffset;

	std::string m_pcNextObjectCreated;
	bool m_bCreateObject;

	//Saving and Loading files
	HWND m_hWindow;
	CLevel* m_pCurrentLevel;
	IFileOpenDialog* m_pFileOpenDialog;
	IFileDialog* m_pFileSaveDialog;
};

//class CDialogEvent : public IFileDialogEvents
//{
//public:
//	void Initialise(CEditorInterface* _pEditor);
//	virtual HRESULT OnFileOk(__RPC__in_opt IFileDialog *pfd){}
//	virtual HRESULT OnFolderChange(__RPC__in_opt IFileDialog *pfd){}
//	virtual HRESULT OnFolderChanging(__RPC__in_opt IFileDialog *pfd){}
//	virtual HRESULT OnOverwrite(__RPC__in_opt IFileDialog *pfd){}
//	virtual HRESULT OnSelectionChange(__RPC__in_opt IFileDialog *pfd){}
//	virtual HRESULT OnShareViolation(__RPC__in_opt IFileDialog *pfd){}
//	virtual HRESULT OnTypeChange(__RPC__in_opt IFileDialog *pfd){}
//
//private:
//	CEditorInterface* m_pEditor;
//};
#endif //__EDITORINTERFACE_H__