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

	struct ShaderParamInfo;
	class IShader;
	typedef IShader*					( *CreateShaderFn_t )( );

	//---------------------------------------------------------------------//

	struct ShaderDescriptor
	{
		ShaderDescriptor() :
			name( nullptr ),
			CreateShaderFn( nullptr ),
			countParameters( 0 ),
			parametersInfo( nullptr )
		{}

		char*					name;
		UInt32_t				countParameters;
		ShaderParamInfo*		parametersInfo;
		CreateShaderFn_t		CreateShaderFn;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_DESCRIPTOR_H

