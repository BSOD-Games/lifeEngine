//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIALPRXOYVAR_INFO_H
#define MATERIALPRXOYVAR_INFO_H

#include "engine/imaterialproxyvar.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct MaterialProxyVarInfo
	{
		const char*					name;
		MATERIAL_PROXY_VAR_TYPE		type;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIALPRXOYVAR_INFO_H