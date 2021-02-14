//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIO_VERTEX_ELEMENT_H
#define STUDIO_VERTEX_ELEMENT_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum VERTEX_ELEMENT_TYPE
	{
		VET_FLOAT,
		VET_UNSIGNED_INT,
		VET_UNSIGNED_BYTE
	};

	//---------------------------------------------------------------------//

	struct StudioVertexElement
	{
		UInt32_t						count;
		VERTEX_ELEMENT_TYPE				type;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STUDIO_VERTEX_ELEMENT_H
