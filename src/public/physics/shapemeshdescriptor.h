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
        int             countVerteces;
        Vector3D_t*     verteces;
        int             countIndeces;
        int*            indeces;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHAPEMESHDESCRIPTOR_H
