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
#include <atlbase.h>

// Local Includes
#include "defines.h"
#include "level.h"
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
, m_pFileOpenDialog(0)
, m_pFileSaveDialog(0)
, m_pCurrentLevel(0)
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
CEditorInterface::Initialise(HWND _hWindow, CLevel* _pLevel)
{
	m_pWindowColours = new D3DXCOLOR[WINDOWSTATE_MAX];
	m_pWindowColours[WINDOWSTATE_OPEN] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
	m_pWindowColours[WINDOWSTATE_CLOSED] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pWindowColours[WINDOWSTATE_MOUSEOVER] = D3DXCOLOR(0.5f, 0.7f, 0.8f, 1.0f);
	m_pWindowColours[WINDOWSTATE_CLOSEBUTTON] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	m_hWindow = _hWindow;
	m_pCurrentLevel = _pLevel;

	return true;
}
/**
*
* CEditorInterface class Process function
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CEditorInterface::Process(float _fDeltaTime)
{
	
}
/**
*
* CEditorInterface class Processes input received
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pKeys Input structure
* @param _fDeltaTime Game time elapsed
*
*/
bool
CEditorInterface::ProcessInput(ID3D11Device* _pDevice, TInputStruct* _pKeys, float _fDeltaTime)
{
	bool bHasReceivedInput = false;
	m_bCreateObject = false;
	m_eEditorState = EDITOR_IDLE;

	float fColourChangeSpeed = 5.0f;

	//Check for input received through button presses or window interaction
	m_bHasChanged = false;
	bool bIsOverButton = false;
	bool bWindowHasCollided = false;
	m_bHasChanged = true;
	for (unsigned int iWindow = 0; iWindow < m_vecWindows.size(); ++iWindow)
	{
		if (m_bIsActive && m_vecWindows[iWindow]->bIsActive)
		{
			//Check if the mouse is within the range of a window
			bWindowHasCollided = HasCollided(_pKeys->vecMouse, m_vecWindows[iWindow]);
			bIsOverButton = false;
			if (HasCollided(_pKeys->vecMouse, m_vecWindows[iWindow]))
			{
				m_vecWindows[iWindow]->targetColour = m_pWindowColours[WINDOWSTATE_MOUSEOVER];
				bHasReceivedInput = true;
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
						if (HasCollided(_pKeys->vecMouse, pCurrentButton))
						{
							bIsOverButton = true;
							if (pCurrentButton->sName == "Close")
							{
								pCurrentButton->targetColour = m_pWindowColours[WINDOWSTATE_CLOSEBUTTON];
							}
							else
							{
								pCurrentButton->targetColour = m_pWindowColours[WINDOWSTATE_MOUSEOVER];
							}
							if (_pKeys->bLeftMouseClick.bPressed && _pKeys->bLeftMouseClick.bPreviousState == false)
							{
								ProcessButtonPressed(_pDevice, m_vecWindows[iWindow], pCurrentButton);
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
			pCurrentButton->pForegroundVertex->colour += (pCurrentButton->targetColour - pCurrentButton->pForegroundVertex->colour) * fColourChangeSpeed * _fDeltaTime;
			pCurrentButton->pBackgroundVertex->colour += (pCurrentButton->targetColour - pCurrentButton->pBackgroundVertex->colour) * fColourChangeSpeed * _fDeltaTime;
		}
		m_vecWindows[iWindow]->pForegroundVertex->colour += (m_vecWindows[iWindow]->targetColour - m_vecWindows[iWindow]->pForegroundVertex->colour) * fColourChangeSpeed * _fDeltaTime;
		m_vecWindows[iWindow]->pBackgroundVertex->colour += (m_vecWindows[iWindow]->targetColour - m_vecWindows[iWindow]->pBackgroundVertex->colour) * fColourChangeSpeed * _fDeltaTime;
		
		//Check if user is currently dragging a window
		if (m_pDraggedWindow != 0)
		{
			if (_pKeys->bLeftMouseClick.bPressed == false)
			{
				m_pDraggedWindow = 0;
			}
			else
			{
				D3DXVECTOR3 vecToMouse = D3DXVECTOR3(_pKeys->vecMouse.x, _pKeys->vecMouse.y, 0.0f) - m_pDraggedWindow->pForegroundVertex->pos;
				m_pDraggedWindow->OffsetWindow(vecToMouse - m_vecInitialDraggedWindowOffset);
			}
		}
		//Check if the user wants to drag this window
		else if (bWindowHasCollided && m_vecWindows[iWindow]->bCanMove && bIsOverButton == false)
		{
			if (_pKeys->bLeftMouseClick.bPressed)
			{
				m_pDraggedWindow = m_vecWindows[iWindow];
				m_vecInitialDraggedWindowOffset = D3DXVECTOR3(_pKeys->vecMouse.x, _pKeys->vecMouse.y, 0.0f) - m_pDraggedWindow->pForegroundVertex->pos;
			}
		}
		else
		{

		}
	}
	return bHasReceivedInput;
}
/**
*
* CEditorInterface class Get the state of the Editor
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the state of the editor
*
*/
EEditorState
CEditorInterface::GetEditorState() const
{
	return m_eEditorState;
}
/**
*
* CEditorInterface class Gets the selected prefab
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rPrefabName Name of the selected prefab
*
*/
std::string&
CEditorInterface::GetSelectedPrefab()
{
	return m_pcNextObjectCreated;
}
/**
*
* CEditorInterface class Switches the editor interface on and off
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bIsActive Editor state
*
*/
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
/**
*
* CEditorInterface class Returns whether the editor is active or not
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
bool 
CEditorInterface::IsActive() const
{
	return m_bIsActive;
}
/**
*
* KEY AREA: CEditorInterface class Opens the "Load level" dialog box and tells the Level class to load the selected level from file
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CEditorInterface::LoadLevel(ID3D11Device* _pDevice)
{
	HRCheck( CoCreateInstance(	CLSID_FileOpenDialog,
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_PPV_ARGS(&m_pFileOpenDialog)),
								L"Could not create OPEN dialog box");
	
	COMDLG_FILTERSPEC tFileType[] =
	{
		{ L"Level File", L"*.xml" }
	};
	m_pFileOpenDialog->SetFileTypes(1, tFileType);
	m_pFileOpenDialog->SetTitle(L"Open Level from File:");
	HRESULT hr = m_pFileOpenDialog->Show(m_hWindow);
	if (SUCCEEDED(hr))
	{
		IShellItem* pShellItem;
		hr = m_pFileOpenDialog->GetResult(&pShellItem);
		wchar_t* pName;
		pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pName);

		int iStrLength = lstrlenW(pName);
		size_t iReturnSize = 0;
		char* pcName = new char[iStrLength + 1];
		const wchar_t* pwName = pName;
		wcsrtombs_s(&iReturnSize, pcName, sizeof(char) * (iStrLength + 1), &pwName, iStrLength, std::mbstate_t());
		pcName[iStrLength] = '\0';
		MessageBox(NULL, pName, L"File Chosen:", MB_OK);
		m_pCurrentLevel->LoadLevel(_pDevice, pcName);

		pShellItem->Release();
		delete[] pcName;
	}
}
/**
*
* KEY AREA: CEditorInterface class Opens the "Save level" dialog box and tells the Level class to save the current level to file
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CEditorInterface::SaveLevel(ID3D11Device* _pDevice)
{
	//Specify file types
	COMDLG_FILTERSPEC tFileType[] =
	{
		{ L"Level File", L"*.xml" }
	};
	//Open 
	HRCheck(CoCreateInstance(CLSID_FileSaveDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_pFileSaveDialog)),
		L"Could not create SAVE dialog box");
	
	//Set default file type
	m_pFileSaveDialog->SetFileTypes(1, tFileType);
	m_pFileSaveDialog->SetTitle(L"Open Level from File:");
	HRESULT hr = m_pFileSaveDialog->Show(m_hWindow);
	if (SUCCEEDED(hr))
	{
		IShellItem* pShellItem;
		hr = m_pFileSaveDialog->GetResult(&pShellItem);
		wchar_t* pName;
		pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pName);

		int iStrLength = lstrlenW(pName);
		size_t iReturnSize = 0;
		char* pcName = new char[iStrLength + 1];
		const wchar_t* pwName = pName;
		wcsrtombs_s(&iReturnSize, pcName, sizeof(char) * (iStrLength + 1), &pwName, iStrLength, std::mbstate_t());
		pcName[iStrLength] = '\0';
		MessageBox(NULL, pName, L"File Chosen:", MB_OK);
		m_pCurrentLevel->SaveLevel(_pDevice, pcName);

		pShellItem->Release();
		delete[] pcName;
	}
}
/**
*
* CEditorInterface class Recreates the vertex and index buffers of the editor interface
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
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
/**
*
* CEditorInterface class Returns true if the mouse position is within the bounds of a button (or window)
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
bool
CEditorInterface::HasCollided(D3DXVECTOR2& _rPoint, TButton* _pButton)
{
	_pButton->vecPosition = _pButton->pForegroundVertex->pos;
	return (_rPoint.x > _pButton->vecPosition.x &&
			_rPoint.x < _pButton->vecPosition.x + _pButton->vecScale.x &&
			_rPoint.y < _pButton->vecPosition.y &&
			_rPoint.y > _pButton->vecPosition.y - _pButton->vecScale.y);
}
/**
*
* KEY AREA: CEditorInterface class Processes a button pressed according to the buttons name
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CEditorInterface::ProcessButtonPressed(ID3D11Device* _pDevice, TWindow* _pWindow, TButton* _pButton)
{
	CAudioPlayer::GetInstance().Play3DSound(SOUND_BUTTONPRESS, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	if (strcmp(_pButton->sName.c_str(), "Close") == 0)
	{
		m_eEditorState = EDITOR_CLOSED;
		_pWindow->SetIsActive(false, m_pWindowColours[WINDOWSTATE_CLOSED]);
	}
	else if (strcmp(_pButton->sName.c_str(), "OpenPrefabWindow") == 0)
	{
		m_eEditorState = EDITOR_IDLE;
		m_vecWindows[1]->SetIsActive(true, m_pWindowColours[WINDOWSTATE_OPEN]);
	}
	else if (strcmp(_pButton->sName.c_str(), "OpenSceneGraph") == 0)
	{
		m_eEditorState = EDITOR_IDLE;
		m_vecWindows[2]->SetIsActive(true, m_pWindowColours[WINDOWSTATE_OPEN]);
	}
	else if (strcmp(_pButton->sName.c_str(), "Create") == 0)
	{
		m_eEditorState = EDITOR_SELECTED;
		m_bCreateObject = true;
		m_pcNextObjectCreated = _pButton->sOptions.c_str();
	}
	else if (strcmp(_pButton->sName.c_str(), "LoadLevel") == 0)
	{
		LoadLevel(_pDevice);
	}
	else if (strcmp(_pButton->sName.c_str(), "SaveLevel") == 0)
	{
		SaveLevel(_pDevice);
	}
	else if (strcmp(_pButton->sName.c_str(), "NewPrefab") == 0)
	{
		//Create a new prefab
	}
}
/**
*
* KEY AREA: CEditorInterface class Loads editor data from an XML file
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CEditorInterface::LoadFromXML(ID3D11Device* _pDevice, CResourceManager* _pResourceManager, CEntityManager* _pEntityManager, char* _pcXMLFilename)
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

	D3DXCOLOR initialColour(0.0f, 0.0f, 0.0f, 0.0f);

	//Loop through windows in the XML file
	for (rapidxml::xml_node<>* pCurrentWindow = pWindows; pCurrentWindow; pCurrentWindow = pCurrentWindow->next_sibling())
	{
		D3DXVECTOR2 vecWindowSize(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
		TWindow* pNewWindow = new TWindow;
		pNewWindow->sName = pCurrentWindow->first_attribute("id")->value();
		pNewWindow->iForegroundTexture = _pResourceManager->GetTextureID(std::string(pCurrentWindow->first_node("foregroundtexture")->value()));
		pNewWindow->iBackgroundTexture = _pResourceManager->GetTextureID(std::string(pCurrentWindow->first_node("backgroundtexture")->value()));

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
		pNewWindow->colour = initialColour;
		pNewWindow->bIsActive = true;
		
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
			pNewButton->iForegroundTexture = _pResourceManager->GetTextureID(std::string(pButtons->first_node("maintexture")->value()));
			pNewButton->iBackgroundTexture = _pResourceManager->GetTextureID(std::string(pButtons->first_node("bgtexture")->value()));

			pNewButton->vecScale = D3DXVECTOR2(	ReadFromString<float>(pButtons->first_node("scale")->first_attribute("x")->value()) * pNewWindow->vecScale.x,
												ReadFromString<float>(pButtons->first_node("scale")->first_attribute("y")->value()) * pNewWindow->vecScale.x);
			if (pButtons->first_node("position"))
			{
				pNewButton->vecPosition = D3DXVECTOR3(	(ReadFromString<float>(pButtons->first_node("position")->first_attribute("x")->value()) * pNewWindow->vecScale.x),
														-(ReadFromString<float>(pButtons->first_node("position")->first_attribute("y")->value()) * pNewWindow->vecScale.y),
														ReadFromString<float>(pButtons->first_node("position")->first_attribute("z")->value())) + 
														pNewWindow->vecPosition;
			}
			else
			{
				//Generate a position for this button
				D3DXVECTOR3 vecButtonOffset = D3DXVECTOR3(	(pNewButton->vecScale.x * iButtonPositionsGenerated * 1.1f) + pNewButton->vecScale.x * 0.5f,
															-pNewButton->vecScale.y * 0.5f - (pNewButton->vecScale.y),
															0.0f);
				pNewButton->vecPosition = pNewWindow->vecPosition + vecButtonOffset;
				++iButtonPositionsGenerated;
			}
			pNewButton->colour = initialColour;
			pNewWindow->vecButtons.push_back(pNewButton);

			//Check if this is the "New Prefab" button
			if (strcmp(pNewButton->sName.c_str(), "NewPrefab") == 0)
			{
				AddPrefabCreationButtons(_pEntityManager, _pResourceManager, pNewWindow, pNewButton);
			}
		}
		m_vecWindows.push_back(pNewWindow);
	}
	//Create sprites in correct rendering order for windows and buttons
	for (int iWindow = static_cast<int>(m_vecWindows.size()) - 1; iWindow >= 0; --iWindow)
	{
		for (int iButton = static_cast<int>(m_vecWindows[iWindow]->vecButtons.size()) - 1; iButton >= 0; --iButton)
		{
			m_vecWindows[iWindow]->vecButtons[iButton]->pBackgroundVertex = CreatePointSprite(_pDevice, m_vecWindows[iWindow]->vecButtons[iButton]->vecPosition + D3DXVECTOR3(0.0f, 0.0f, 0.1f), m_vecWindows[iWindow]->vecButtons[iButton]->vecScale, m_vecWindows[iWindow]->vecButtons[iButton]->colour, 0.0f, m_vecWindows[iWindow]->vecButtons[iButton]->iBackgroundTexture);
			m_vecWindows[iWindow]->vecButtons[iButton]->pForegroundVertex = CreatePointSprite(_pDevice, m_vecWindows[iWindow]->vecButtons[iButton]->vecPosition + D3DXVECTOR3(0.0f, 0.0f, 0.05f), m_vecWindows[iWindow]->vecButtons[iButton]->vecScale, m_vecWindows[iWindow]->vecButtons[iButton]->colour, 0.0f, m_vecWindows[iWindow]->vecButtons[iButton]->iForegroundTexture);
		}
		m_vecWindows[iWindow]->pBackgroundVertex = CreatePointSprite(_pDevice, m_vecWindows[iWindow]->vecPosition + D3DXVECTOR3(0.0f, 0.0f, 0.1f), m_vecWindows[iWindow]->vecScale, m_vecWindows[iWindow]->colour, 0.0f, m_vecWindows[iWindow]->iBackgroundTexture);
		m_vecWindows[iWindow]->pForegroundVertex = CreatePointSprite(_pDevice, m_vecWindows[iWindow]->vecPosition + D3DXVECTOR3(0.0f, 0.0f, 0.05f), m_vecWindows[iWindow]->vecScale, m_vecWindows[iWindow]->colour, 0.0f, m_vecWindows[iWindow]->iForegroundTexture);
	}

	delete[] pcBuffer;
	pcBuffer = 0;
}
/**
*
* KEY AREA: CEditorInterface class Adds Prefab Creation Buttons to the current window
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CEditorInterface::AddPrefabCreationButtons(CEntityManager* _pEntityManager, CResourceManager* _pResourceManager, TWindow* _pWindow, TButton* _pNewPrefabButton)
{
	int iNumPrefabs = _pEntityManager->GetPrefabCount();
	TPrefabOptions* pCurrentPrefab = 0;
	int iButtonHeight = 0;
	int iButtonWidth = 0;
	int iMaxWidthCount = 5;
	float fButtonGap = 1.1f;

	for (int iPrefab = 0; iPrefab < iNumPrefabs; ++iPrefab)
	{
		++iButtonWidth;
		if (iButtonWidth >= iMaxWidthCount)
		{
			iButtonWidth = 0;
			++iButtonHeight;
		}
		pCurrentPrefab = _pEntityManager->GetPrefabOptions(iPrefab);

		//Create a new button for this prefab type
		TButton* pNewPrefab = new TButton();
		*pNewPrefab = *_pNewPrefabButton;

		pNewPrefab->vecPosition += D3DXVECTOR3(_pNewPrefabButton->vecScale.x * fButtonGap * iButtonWidth, _pNewPrefabButton->vecScale.y * fButtonGap * -iButtonHeight, 0.0f);
		//Set the button type to "Create"
		pNewPrefab->sName = "Create";
		//Set the button options to the name of the prefab
		pNewPrefab->sOptions = pCurrentPrefab->pcPrefabName;
		pNewPrefab->iForegroundTexture = _pResourceManager->GetTextureID(pCurrentPrefab->pTexture);
		_pWindow->vecButtons.push_back(pNewPrefab);
	}
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