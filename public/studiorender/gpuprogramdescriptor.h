//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef GPUPROGRAM_DESCRIPTOR_H
#define GPUPROGRAM_DESCRIPTOR_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct GPUProgramDescriptor
	{
		GPUProgramDescriptor() :
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

#endif // !GPUPROGRAM_DESCRIPTOR_H

