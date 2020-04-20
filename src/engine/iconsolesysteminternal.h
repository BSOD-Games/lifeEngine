//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ICONSOLESYSTEM_INTERNAL_H
#define ICONSOLESYSTEM_INTERNAL_H

#include "engine/iconsolesystem.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IEngine;

    //---------------------------------------------------------------------//

    class IConsoleSystemInternal : public IConsoleSystem
    {
    public:
        virtual void		Initialize( const char* LogFile ) = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ICONSOLESYSTEM_INTERNAL_H

