//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CONE_H
#define CONE_H

#include "baseprimitive.h"

//----------------------------------------------------------------------//

namespace le
{
    //----------------------------------------------------------------------//

    class Cone : public BasePrimitive
    {
    public:
        Cone();
        ~Cone();

        virtual void            Create();

        virtual UInt32_t	    GetCountIndeces() const;
	    virtual	UInt32_t	    GetStartIndex() const;
    };

    //----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !CONE_H