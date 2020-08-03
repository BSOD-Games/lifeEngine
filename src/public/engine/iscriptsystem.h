//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISCRIPTSYSTEM_H
#define ISCRIPTSYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IScript;

    //---------------------------------------------------------------------//

    class IScriptSystem
    {
    public:
        virtual ~IScriptSystem() {}
        virtual IScript*		CreateScript( const char* Path ) const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISCRIPTSYSTEM_H
