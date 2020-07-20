//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIALSYSTEMINTERNAL_H
#define IMATERIALSYSTEMINTERNAL_H

#include "engine/imaterialsystem.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

	class IEngine;
	class IMaterial;

    //---------------------------------------------------------------------//

    class IMaterialSystemInternal : public IMaterialSystem
    {
    public:
        virtual ~IMaterialSystemInternal() {}
		virtual bool				Initialize( IEngine* Engine ) = 0;
		virtual void				Update() = 0;
		virtual void				SubmitUpdate( IMaterial* Material ) = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALSYSTEMINTERNAL_H
