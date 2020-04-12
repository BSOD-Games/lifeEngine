//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_SHAPEMESHDESCRIPTOR_H
#define API_SHAPEMESHDESCRIPTOR_H

#include "../common/types.h"
#include "../mathlib/mathlib.h"

//---------------------------------------------------------------------//

typedef struct
{
	int             countVerteces;
	vec3f_t*		verteces;
	int             countIndeces;
	int*            indeces;
} shapeMeshDescriptor_t;

//---------------------------------------------------------------------//

#endif // !API_SHAPEMESHDESCRIPTOR_H
