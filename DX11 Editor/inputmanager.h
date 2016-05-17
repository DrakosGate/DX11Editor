

#pragma once

#include <vector>

#include "mathlibrary.h"

namespace Listeners
{
	class InputListener;
}

class InputManager
{
public:
	static InputManager* GetInstance();
	void Delete();
	~InputManager();

	bool ProcessInput( HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam );
	void AddListener( Listeners::InputListener* _pNewListener );
	void RemoveListener( Listeners::InputListener* _pListener );

private:
	InputManager();

	static InputManager* s_pInputManager;
	std::vector< Listeners::InputListener* > m_vecListeners;
	Math::Vector2 m_previousMousePos;
};