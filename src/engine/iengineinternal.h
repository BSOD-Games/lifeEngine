//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IENGINE_INTERNAL_H
#define IENGINE_INTERNAL_H

#include "engine/lifeengine.h"
#include "engine/iengine.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	typedef void			( *CriticalErrorCallbackFn_t )( const char* Message );

	//---------------------------------------------------------------------//

	class IEngineInternal : public IEngine
	{
	public:
		virtual bool		Initialize( WindowHandle_t WindowHandle = nullptr ) = 0;
		virtual bool		LoadGame( const char* DirGame ) = 0;
		virtual void		UnloadGame() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IENGINE_INTERNAL_H