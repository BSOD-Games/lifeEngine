//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IGAMEMODE_H
#define IGAMEMODE_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;

	//---------------------------------------------------------------------//

	class IGameMode
	{
	public:
        virtual ~IGameMode() {}
		virtual bool				Initialize( IEngine* Engine, UInt32_t CountArguments, const char** Arguments ) = 0;
		virtual bool				LoadGame( const char* PathLevel ) = 0;
		virtual void				UnloadGame() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IGAMEMODE_H

