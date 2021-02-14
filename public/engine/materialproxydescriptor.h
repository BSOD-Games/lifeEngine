//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIALPROXY_DESCRIPTOR_H
#define MATERIALPROXY_DESCRIPTOR_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct MaterialProxyVarInfo;
	class IMaterialProxy;
	typedef IMaterialProxy*					( *CreateMaterialProxyFn_t )( );

	//---------------------------------------------------------------------//

	struct MaterialProxyDescriptor
	{
		MaterialProxyDescriptor() :
			name( nullptr ),
			CreateMaterialProxyFn( nullptr ),
			parametersInfo( nullptr ),
			countParameters( 0 )
		{}

		char*						name;
		UInt32_t					countParameters;
		MaterialProxyVarInfo*		parametersInfo;
		CreateMaterialProxyFn_t		CreateMaterialProxyFn;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_DESCRIPTOR_H

