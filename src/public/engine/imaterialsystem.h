//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIALSYSTEM_H
#define IMATERIALSYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

	class IShaderFactory;
	class IMaterialProxyFactory;

    //---------------------------------------------------------------------//

    class IMaterialSystem
    {
    public:
        virtual ~IMaterialSystem() {}
		virtual IMaterialProxyFactory*			GetMaterialProxyFactory() const = 0;
		virtual IShaderFactory*					GetShaderFactory() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALSYSTEM_H
