
#include "inputmanager.h"
#include "inputlistener.h"

InputManager* InputManager::s_pInputManager = 0;

InputManager* 
InputManager::GetInstance( )
{
	if ( s_pInputManager == nullptr )
	{
		s_pInputManager = new InputManager();
	}
	return s_pInputManager;
}
void
InputManager::Delete( )
{
	delete s_pInputManager;
}

InputManager::~InputManager( )
{
	m_vecListeners.clear( );
}

bool
InputManager::ProcessInput( HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam )
{
	bool input_used = true;
	switch ( _uiMsg )
	{
	case WM_KEYDOWN:
	{
		for ( auto iter = m_vecListeners.begin( ); iter != m_vecListeners.end( ); ++iter )
		{
			( *iter )->OnKeyDown( LOWORD( _wParam ) );
		}
		break;
	}
	case WM_KEYUP:
	{
		for ( auto iter = m_vecListeners.begin( ); iter != m_vecListeners.end( ); ++iter )
		{
			( *iter )->OnKeyUp( LOWORD( _wParam ) );
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		const Math::Vector2 newMousePosition( LOWORD( _lParam ), HIWORD( _lParam ) );
		Math::Vector2 mouseChange = Math::Sub( newMousePosition, m_previousMousePos );
		mouseChange = Math::Vector2( std::max( -100.0f, std::min( mouseChange.X(), 100.0f ) ), std::max( -100.0f, std::min( mouseChange.Y(), 100.0f ) ) );
	
		for (auto iter = m_vecListeners.begin(); iter != m_vecListeners.end(); ++iter)
		{
			(*iter)->OnMouseMove( mouseChange );
		}

		m_previousMousePos = newMousePosition;
		break;
	}
	default:
		input_used = false;
		break;
	}
		
	return input_used;
}

void
InputManager::AddListener( Listeners::InputListener* _pNewListener )
{
	m_vecListeners.push_back( _pNewListener );
}

void
InputManager::RemoveListener( Listeners::InputListener* _pListener )
{
	for ( auto iter = m_vecListeners.begin( ); iter != m_vecListeners.end( ); ++iter )
	{
		if ( ( *iter ) == _pListener )
		{
			m_vecListeners.erase( iter );
			break;
		}
	}
}


InputManager::InputManager( ) : m_previousMousePos( 0, 0 )
{
	
}
