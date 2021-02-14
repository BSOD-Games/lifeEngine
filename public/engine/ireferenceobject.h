//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IREFERENCE_OBJECT_H
#define IREFERENCE_OBJECT_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class IReferenceObject
    {
    public:
        virtual ~IReferenceObject() {}
        virtual void                IncrementReference() = 0;
        virtual void                DecrementReference() = 0;
        virtual void                Release() = 0;

        virtual UInt32_t            GetCountReferences() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IREFERENCE_OBJECT_H
