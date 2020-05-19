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

	class IShader;
	typedef IShader*					( *CreateShaderFn_t )( );

	//---------------------------------------------------------------------//

	class IShaderFactory
	{
	public:
        virtual ~IShaderFactory() {}
		virtual void					Register( const char* Name, CreateShaderFn_t CreateShader ) = 0;
		virtual void					Unregister( const char* Name ) = 0;
		virtual IShader*				Create( const char* Name ) = 0;

		virtual UInt32_t				GetCountShaders() const = 0;
		virtual const char*				GetShader( UInt32_t Index ) const = 0;
		virtual const char**			GetShaders() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISHADERFACTORY_H

