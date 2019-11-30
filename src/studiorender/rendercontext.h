//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "engine/lifeEngine.h"

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//

	typedef void*			ContextDescriptor_t;

	//----------------------------------------------------------------------//

	struct SettingsContext;

	//----------------------------------------------------------------------//

	class RenderContext
	{
	public:
		RenderContext();
		~RenderContext();

		bool					Create( WindowHandle_t WindowHandle, const SettingsContext& SettingsContext );
		void					MakeCurrent();	
		void					Destroy();
		void					SwapBuffers();

		void					SetVerticalSync( bool IsEnable = true );
		inline bool				IsCreated() const { return isCreated; }

	private:
		bool					isCreated;	

		ContextDescriptor_t		contextDescriptor;		
		WindowHandle_t			windowHandle;			
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !RENDER_CONTEXT_H
