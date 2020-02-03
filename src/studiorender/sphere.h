//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SPHERE_H
#define SPHERE_H

#include "baseprimitive.h"

//----------------------------------------------------------------------//

namespace le
{
    //----------------------------------------------------------------------//

    class Sphere : public BasePrimitive
    {
    public:
        Sphere();
        ~Sphere();

        void                Create( float Radius );

        UInt32_t			GetCountIndeces() const;
		UInt32_t			GetStartIndex() const;
    };

    //----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !SPHERE_H