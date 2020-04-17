//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHAPEMESHDESCRIPTOR_H
#define SHAPEMESHDESCRIPTOR_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    struct ShapeMeshDescriptor
    {
        UInt32_t        countVerteces;
        Vector3D_t*     verteces;
        UInt32_t        countIndeces;
        UInt32_t*       indeces;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHAPEMESHDESCRIPTOR_H
