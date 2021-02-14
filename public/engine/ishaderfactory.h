//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISHADERFACTORY_H
#define ISHADERFACTORY_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ShaderDescriptor;
	class IShader;

	//---------------------------------------------------------------------//

	class IShaderFactory
	{
	public:
        virtual ~IShaderFactory() {}
		virtual void						Register( const ShaderDescriptor& ShaderDescriptor ) = 0;
		virtual void						Unregister( const char* Name ) = 0;
		virtual IShader*					Create( const char* Name ) = 0;

		virtual UInt32_t					GetCountShaders() const = 0;
		virtual ShaderDescriptor			GetShader( UInt32_t Index ) const = 0;
		virtual ShaderDescriptor*			GetShaders() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISHADERFACTORY_H

