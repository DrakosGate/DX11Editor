//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   model.cpp
//  Description :   Code for class CEditorInterface
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <D3D11.h>
#include <rapidxml_utils.hpp>

// Local Includes
#include "defines.h"
#include "shader.h"
#include "resourcemanager.h"
#include "entitymanager.h"
#include "audioplayer.h"

// This Include
#include "editorinterface.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CEditorInterface class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CEditorInterface::CEditorInterface()
: m_bIsActive(false)
, m_pWindowColours(0)
, m_pDraggedWindow(0)
{

}

/**
*
* CEditorInterface class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CEditorInterface::~CEditorInterface()
{
	if (m_pWindowColours)
	{
		delete[] m_pWindowColours;
		m_pWindowColours = 0;
	}
	for (unsigned int iWindow = 0; iWindow < m_vecWindows.size(); ++iWindow)
	{
		for (unsigned int iButton = 0; iButton < m_vecWindows[iWindow]->vecButtons.size(); ++iButton)
		{
			delete m_vecWindows[iWindow]->vecButtons[iButton];
			m_vecWindows[iWindow]->vecButtons[iButton] = 0;
		}
		m_vecWindows[iWindow]->vecButtons.clear();

		delete m_vecWindows[iWindow];
		m_vecWindows[iWindow] = 0;
	}
	m_vecWindows.clear();
}

/**
*
* CEditorInterface class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @return Returns true
*
*/
bool 
CEditorInterface::Initialise()
{
	m_pWindowColours = new D3DXCOLOR[WINDOWSTATE_MAX];
	m_pWindowColours[WINDOWSTATE_OPEN] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pWindowColours[WINDOWSTATE_CLOSED] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pWindowColours[WINDOWSTATE_MOUSEOVER] = D3DXCOLOR(0.5f, 0.7f, 0.8f, 1.0f);
	return true;
}
void
CEditorInterface::ProcessInput(TInputStruct& _pKeys, float _fDT)
{
	float fColourChangeSpeed = 5.0f;
	if (_pKeys.bTilde.bPressed && _pKeys.bTilde.bPreviousState == false)
	{
		ToggleEditor(!m_bIsActive);
	}
	
	m_bHasChanged = false;
	bool bIsOverButton = false;
	bool bWindowHasCollided = false;
	m_bHasChanged = true;
	for (unsigned int iWindow = 0; iWindow < m_vecWindows.size(); ++iWindow)
	{
		if (m_bIsActive && m_vecWindows[iWindow]->bIsActive)
		{
			//Check if the mouse is within the range of a window
			bWindowHasCollided = HasCollided(_pKeys.vecMouse, m_vecWindows[iWindow]);
			bIsOverButton = false;
			if (HasCollided(_pKeys.vecMouse, m_vecWindows[iWindow]))
			{
				m_vecWindows[iWindow]->targetColour = m_pWindowColours[WINDOWSTATE_MOUSEOVER];
			}
			else
			{
				m_vecWindows[iWindow]->targetColour = m_pWindowColours[WINDOWSTATE_OPEN];
			}
		}
		//Check for buttons within this window
		for (unsigned int iButton = 0; iButton < m_vecWindows[iWindow]->vecButtons.size(); ++iButton)
		{
			TButton* pCurrentButton = m_vecWindows[iWindow]->vecButtons[iButton];
			if (m_vecWindows[iWindow]->bIsActive)
			{
				if (m_bIsActive)
				{
					if (bWindowHasCollided)
					{
						if (HasCollided(_pKeys.vecMouse, pCurrentButton))
						{
							bIsOverButton = true;
							pCurrentButton->targetColour = m_pWindowColours[WINDOWSTATE_MOUSEOVER];
							if (_pKeys.bLeftMouseClick.bPressed && _pKeys.bLeftMouseClick.bPreviousState == false)
							{
								ProcessButtonPressed(m_vecWindows[iWindow], pCurrentButton);
								break;
							}
						}
						else
						{
							pCurrentButton->targetColour = m_pWindowColours[WINDOWSTATE_OPEN];
						}
					}
				}
			}
			pCurrentButton->pForegroundVertex->colour += (pCurrentButton->targetColour - pCurrentButton->pForegroundVertex->colour) * fColourChangeSpeed * _fDT;
			pCurrentButton->pBackgroundVertex->colour += (pCurrentButton->targetColour - pCurrentButton->pBackgroundVertex->colour) * fColourChangeSpeed * _fDT;
		}
		m_vecWindows[iWindow]->pForegroundVertex->colour += (m_vecWindows[iWindow]->targetColour - m_vecWindows[iWindow]->pForegroundVertex->colour) * fColourChangeSpeed * _fDT;
		m_vecWindows[iWindow]->pBackgroundVertex->colour += (m_vecWindows[iWindow]->targetColour - m_vecWindows[iWindow]->pBackgroundVertex->colour) * fColourChangeSpeed * _fDT;
		
		//Check if user is currently dragging a window
		if (m_pDraggedWindow != 0)
		{
			if (_pKeys.bLeftMouseClick.bPressed == false)
			{
				m_pDraggedWindow = 0;
			}
			else
			{
				D3DXVECTOR3 vecToMouse = D3DXVECTOR3(_pKeys.vecMouse.x, _pKeys.vecMouse.y, 0.0f) - m_pDraggedWindow->pForegroundVertex->pos;
				m_pDraggedWindow->OffsetWindow(vecToMouse - m_vecInitialDraggedWindowOffset);
			}
		}
		//Check if the user wants to drag this window
		else if (bWindowHasCollided && m_vecWindows[iWindow]->bCanMove && bIsOverButton == false)
		{
			if (_pKeys.bLeftMouseClick.bPressed)
			{
				m_pDraggedWindow = m_vecWindows[iWindow];
				m_vecInitialDraggedWindowOffset = D3DXVECTOR3(_pKeys.vecMouse.x, _pKeys.vecMouse.y, 0.0f) - m_pDraggedWindow->pForegroundVertex->pos;
			}
		}
		else
		{

		}
	}
}
void
CEditorInterface::ToggleEditor(bool _bIsActive)
{
	m_bIsActive = _bIsActive;
	//Set activity of all windows
	for (unsigned int iWindow = 0; iWindow < m_vecWindows.size(); ++iWindow)
	{
		if (m_bIsActive)
		{
			m_vecWindows[iWindow]->SetIsActive(_bIsActive, m_pWindowColours[WINDOWSTATE_OPEN]);
		}
		else
		{
			m_vecWindows[iWindow]->SetIsActive(_bIsActive, m_pWindowColours[WINDOWSTATE_CLOSED]);
		}
	}
}
void 
CEditorInterface::RefreshBuffers(ID3D11Device* _pDevice)
{
	if (m_bHasChanged)
	{
		ReleaseCOM(m_pVertexBuffer);
		ReleaseCOM(m_pIndexBuffer);
		
		CreateVertexBuffer(_pDevice);
		CreateIndexBuffer(_pDevice);
	}
}
bool
CEditorInterface::HasCollided(D3DXVECTOR2& _rPoint, TButton* _pButton)
{
	_pButton->vecPosition = _pButton->pForegroundVertex->pos;
	return (_rPoint.x > _pButton->vecPosition.x &&
			_rPoint.x < _pButton->vecPosition.x + _pButton->vecScale.x &&
			_rPoint.y < _pButton->vecPosition.y &&
			_rPoint.y > _pButton->vecPosition.y - _pButton->vecScale.y);
}
void
CEditorInterface::ProcessButtonPressed(TWindow* _pWindow, TButton* _pButton)
{
	CAudioPlayer::GetInstance().Play3DSound(SOUND_BUTTONPRESS, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	if (strcmp(_pButton->sName.c_str(), "Close") == 0)
	{
		_pWindow->SetIsActive(false, m_pWindowColours[WINDOWSTATE_CLOSED]);
	}
	if (strcmp(_pButton->sName.c_str(), "OpenPrefabWindow") == 0)
	{
		m_vecWindows[1]->SetIsActive(true, m_pWindowColours[WINDOWSTATE_OPEN]);
	}
	if (strcmp(_pButton->sName.c_str(), "Create") == 0)
	{
		m_bCreateObject = true;
		m_pcNextObjectCreated = _pButton->sOptions.c_str();
	}
}
void
CEditorInterface::CheckForNewObjects(ID3D11Device* _pDevice, CEntityManager* _pEntityManager, CShader* _pObjectShader, CRenderEntity* _pSpawnPos)
{
	if (m_bCreateObject)
	{
		//_pEntityManager->InstantiatePrefab(_pDevice, m_sNextObjectCreated.c_str(), _pObjectShader, )
	}
}
/**
*
* CEditorInterface class Loads editor data from an XML file
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CEditorInterface::LoadFromXML(ID3D11Device* _pDevice, CResourceManager* _pResourceManager, char* _pcXMLFilename)
{
	int iMaxMessageSize = 128;
	char* pcBuffer = new char[iMaxMessageSize];
	printf("==== Loading Editor interface from %s ====\n", _pcXMLFilename);

	//Open file containing resource information
	rapidxml::file<> xmlFile(_pcXMLFilename);
	rapidxml::xml_document<> xmlDoc;

	//Parse file string
	xmlDoc.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* pRoot = xmlDoc.first_node();

	//Find root node
	rapidxml::xml_node<>* pWindows = pRoot->first_node("window");

	//Loop through models
	for (rapidxml::xml_node<>* pCurrentWindow = pWindows; pCurrentWindow; pCurrentWindow = pCurrentWindow->next_sibling())
	{
		D3DXVECTOR2 vecWindowSize(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
		TWindow* pNewWindow = new TWindow;
		pNewWindow->sName = pCurrentWindow->first_attribute("id")->value();
		int iForegroundID = _pResourceManager->GetTextureID(pCurrentWindow->first_node("foregroundtexture")->value());
		int iBackgroundID = _pResourceManager->GetTextureID(pCurrentWindow->first_node("backgroundtexture")->value());

		pNewWindow->vecScale = D3DXVECTOR2(
			ReadFromString<float>(pCurrentWindow->first_node("scale")->first_attribute("x")->value()) * vecWindowSize.x,
			ReadFromString<float>(pCurrentWindow->first_node("scale")->first_attribute("y")->value()) * vecWindowSize.y);
		if (pCurrentWindow->first_node("position"))
		{
			pNewWindow->vecPosition = D3DXVECTOR3(
				ReadFromString<float>(pCurrentWindow->first_node("position")->first_attribute("x")->value()) * vecWindowSize.x - (vecWindowSize.x * 0.5f),
				-ReadFromString<float>(pCurrentWindow->first_node("position")->first_attribute("y")->value()) * vecWindowSize.y + (vecWindowSize.y * 0.5f),
				ReadFromString<float>(pCurrentWindow->first_node("position")->first_attribute("z")->value()));
		}
		else
		{
			
		}
		//Check if this window is able to be moved by the user
		pNewWindow->bCanMove = (strcmp(pCurrentWindow->first_node("canmove")->value(), "true") == 0);
		pNewWindow->colour = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pNewWindow->bIsActive = true;

		pNewWindow->pBackgroundVertex = CreatePointSprite(_pDevice, pNewWindow->vecPosition, pNewWindow->vecScale, pNewWindow->colour, 0.0f, iBackgroundID);
		pNewWindow->pForegroundVertex = CreatePointSprite(_pDevice, pNewWindow->vecPosition, pNewWindow->vecScale, pNewWindow->colour, 0.0f, iForegroundID);
		
		//Find buttons belonging to this window
		rapidxml::xml_node<>* pButtons = pCurrentWindow->first_node("button");
		int iButtonPositionsGenerated = 0;
		for (pButtons; pButtons; pButtons = pButtons->next_sibling())
		{
			TButton* pNewButton = new TButton;
			pNewButton->sName = pButtons->first_attribute("id")->value();
			if (pButtons->first_node("options"))
			{
				pNewButton->sOptions = pButtons->first_node("options")->value();
			}
			int iMainTexture = _pResourceManager->GetTextureID(pButtons->first_node("maintexture")->value());
			int iBackgroundTexture = _pResourceManager->GetTextureID(pButtons->first_node("bgtexture")->value());

			pNewButton->vecScale = D3DXVECTOR2(
				ReadFromString<float>(pButtons->first_node("scale")->first_attribute("x")->value()) * pNewWindow->vecScale.x,
				ReadFromString<float>(pButtons->first_node("scale")->first_attribute("y")->value()) * pNewWindow->vecScale.x);
			if (pButtons->first_node("position"))
			{
				pNewButton->vecPosition = D3DXVECTOR3(
					(ReadFromString<float>(pButtons->first_node("position")->first_attribute("x")->value()) * pNewWindow->vecScale.x),
					-(ReadFromString<float>(pButtons->first_node("position")->first_attribute("y")->value()) * pNewWindow->vecScale.y),
					ReadFromString<float>(pButtons->first_node("position")->first_attribute("z")->value())) + 
					pNewWindow->vecPosition;
			}
			else
			{
				//Generate a position for this button
				D3DXVECTOR3 vecButtonOffset = D3DXVECTOR3(	(pNewButton->vecScale.x * iButtonPositionsGenerated * 1.01f) + pNewButton->vecScale.x * 0.5f,
															-pNewButton->vecScale.y * 0.5f,
															0.0f);
				pNewButton->vecPosition = pNewWindow->vecPosition + vecButtonOffset;
				++iButtonPositionsGenerated;
			}

			pNewButton->pBackgroundVertex = CreatePointSprite(_pDevice, pNewButton->vecPosition, pNewButton->vecScale, pNewWindow->colour, 0.0f, iBackgroundTexture);
			pNewButton->pForegroundVertex = CreatePointSprite(_pDevice, pNewButton->vecPosition, pNewButton->vecScale, pNewWindow->colour, 0.0f, iMainTexture);
			
			pNewWindow->vecButtons.push_back(pNewButton);
		}
		m_vecWindows.push_back(pNewWindow);
	}
	delete[] pcBuffer;
	pcBuffer = 0;
}
/**
*
* CEditorInterface class AddPointSprite
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
TPointSpriteVertex*
CEditorInterface::CreatePointSprite(ID3D11Device* _pDevice, D3DXVECTOR3& _rPosition, D3DXVECTOR2& _rScale, D3DXCOLOR& _rColour, float _fRotation, int _iTextureID)
{
	++m_iVertexCount;
	++m_iIndexCount;

	TPointSpriteVertex* pNewVertex = new TPointSpriteVertex(_rPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f), _rScale, _rColour, _fRotation, _iTextureID);
	m_pPointSpriteVertices.push_back(pNewVertex);

	// Create the index buffer
	delete[] m_pIndices;
	m_pIndices = new unsigned int[m_iIndexCount];
	for (int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
	{
		m_pIndices[iIndex] = iIndex;
	}

	ReleaseCOM(m_pVertexBuffer);
	ReleaseCOM(m_pIndexBuffer);

	CreateVertexBuffer(_pDevice);
	CreateIndexBuffer(_pDevice);

	//Return the latest point sprite created
	return pNewVertex;
}
/**
*
* CEditorInterface class AddPointSprite
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CEditorInterface::SendTextureDataToShader(ID3D11DeviceContext* _pDevContext)
{
	//ID3D11ShaderResourceView* const texture[1] = { GetDiffuseMap() };
	//_pDevContext->PSSetShaderResources(0, 1, texture);
}