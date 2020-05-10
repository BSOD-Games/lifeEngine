//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISHADERMANAGER_H
#define ISHADERMANAGER_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IShader;
	class IGPUProgram;
	typedef IShader*		( *CreateShaderFn_t )();	

	//---------------------------------------------------------------------//

	class IShaderManager
	{
	public:
		virtual ~IShaderManager() {}
		virtual void			RegisterShaderDescriptor( const char* Name, CreateShaderFn_t CreateShader ) = 0;
		virtual void			UnregisterShaderDescriptor( const char* Name ) = 0;
		virtual IShader*		FindShaderDescriptor( const char* Name ) = 0;
		virtual IGPUProgram*	LoadShader( const char* Name, UInt32_t Flags ) = 0;
		virtual void			UnloadShader( const char* Name, UInt32_t Flags ) = 0;
		
		virtual UInt32_t		GetCountShaders() const = 0;
		virtual IGPUProgram*	GetShader( const char* Name, UInt32_t Flags ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISHADERMANAGER_H

