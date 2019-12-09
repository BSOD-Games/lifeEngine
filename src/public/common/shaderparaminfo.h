//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADERPARAMINFO_H
#define SHADERPARAMINFO_H

#include "materialsystem/imaterialvar.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ShaderParamInfo
	{
		const char*				name;
		const char*				helpString;
		const char*				defaultString;
		MATERIAL_VAR_TYPE		typeValue;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADERPARAMINFO_H