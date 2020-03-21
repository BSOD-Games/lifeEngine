//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IENTITY_H
#define IENTITY_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class ILevel;

    //---------------------------------------------------------------------//

    class IEntity
    {
    public:
        virtual ~IEntity() {}
        virtual void				Update() = 0;
        virtual void				KeyValue( const char* Key, const char* Value ) = 0;

        virtual void				SetLevel( ILevel* Level ) = 0;
        virtual void				SetPosition( const Vector3D_t& Position ) = 0;

        virtual ILevel*				GetLevel() const = 0;
        virtual const Vector3D_t&	GetPosition() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IENTITY_H

