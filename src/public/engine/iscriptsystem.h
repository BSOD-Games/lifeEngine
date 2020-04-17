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

    class IFactory;
    class IScript;

    //---------------------------------------------------------------------//

    class IScriptSystem
    {
    public:
        virtual ~IScriptSystem() {}
        virtual void			RegisterFunction( const char* Name, void* Value ) = 0;
        virtual void                    RegisterVar( const char* Name, void* Value ) = 0;
        virtual void                    UnregisterFunction( const char* Name ) = 0;
        virtual void                    UnregisterVar( const char* Name ) = 0;

        virtual IFactory*		GetFactory() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISCRIPTSYSTEM_H
