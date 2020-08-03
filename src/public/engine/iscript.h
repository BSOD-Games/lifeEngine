//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISCRIPT_H
#define ISCRIPT_H

#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    struct ScriptDescriptor;

    //---------------------------------------------------------------------//

    class IScript : public IReferenceObject
    {
    public:
        virtual ~IScript() {}
        virtual void            Start() = 0;
        virtual void            Update() = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISCRIPT_H
