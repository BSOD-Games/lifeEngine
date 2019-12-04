//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADER_DESCRIPTOR_H
#define SHADER_DESCRIPTOR_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ShaderDescriptor
	{
		ShaderDescriptor() :
			vertexShaderSource( nullptr ),
			geometryShaderSource( nullptr ),
			fragmentShaderSource( nullptr )
		{}

		const char*				vertexShaderSource;
		const char*				geometryShaderSource;
		const char*				fragmentShaderSource;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_DESCRIPTOR_H

