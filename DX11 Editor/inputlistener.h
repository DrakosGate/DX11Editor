

#pragma once

#include <windows.h>
#include "mathlibrary.h"

namespace Listeners
{
	
	class InputListener
	{
	public:
		InputListener( ){}
		~InputListener( ){}

		virtual void OnKeyDown( const unsigned int _iKeyID ) {}
		virtual void OnKeyUp( const unsigned int _iKeyID ) {}
		virtual void OnMouseButtonDown( const unsigned int _iKeyID ) {}
		virtual void OnMouseButtonUp( const unsigned int _iKeyID ) {}
		virtual void OnMouseMove( const Math::Vector2& _offset ) {}
	};

}