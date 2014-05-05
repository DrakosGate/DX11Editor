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
// Constants

// Prototypes
struct ID3D11DeviceContext;

class CShader;
class CEntityManager;
class CResourceManager;

class CEditorInterface : public CPointSprite
{
	//Member functions
public:
	CEditorInterface();
	virtual ~CEditorInterface();

	virtual bool Initialise(CEntityManager* _pEntityManager);
	virtual void ProcessInput(TInputStruct& _pKeys, float _fDT);

	virtual void ToggleEditor(bool _bIsActive);
	virtual void RefreshBuffers(ID3D11Device* _pDevice);
	virtual bool HasCollided(D3DXVECTOR2& _rPoint, TButton* _pButton);
	virtual void ProcessButtonPressed(TWindow* _pWindow, TButton* _pButton);

	virtual void LoadFromXML(ID3D11Device* _pDevice, CResourceManager* _pResourceManager, char* _pcXMLFilename);
	virtual TPointSpriteVertex* CreatePointSprite(ID3D11Device* _pDevice, D3DXVECTOR3& _rPosition, D3DXVECTOR2& _rScale, D3DXCOLOR& _rColour, float _fRotation, int _iTextureID);

	virtual void SendTextureDataToShader(ID3D11DeviceContext* _pDevContext);

private:
	CEditorInterface(const CEditorInterface& _krInstanceToCopy);
	const CEditorInterface& operator =(const CEditorInterface& _krInstanceToCopy);

	//Member variables
protected:
	std::vector<TWindow*> m_vecWindows;
	CEntityManager* m_pEntityManager;

	D3DXCOLOR* m_pWindowColours;
	bool m_bHasChanged;
	bool m_bIsActive;

	TWindow* m_pDraggedWindow;
	D3DXVECTOR3 m_vecInitialDraggedWindowOffset;
};

#endif //__EDITORINTERFACE_H__