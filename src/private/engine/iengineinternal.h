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

#include "common/types.h"
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
        virtual bool            Initialize( const char* EngineDirectory, const char* LogFile = "console.log", bool IsEditor = false ) = 0;

        virtual const char*     GetEngineDirectory() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IENGINE_INTERNAL_H
