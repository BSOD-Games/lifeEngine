//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADER_FACTORY_H
#define SHADER_FACTORY_H

#include <string>
#include <vector>
#include <unordered_map>


#include "engine/ishaderfactory.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ShaderFactory : public IShaderFactory
	{
	public:
		// IShaderFactory
		virtual void			Register( const char* Name, CreateShaderFn_t CreateShader );
		virtual void			Unregister( const char* Name );
		virtual IShader*		Create( const char* Name );

		virtual UInt32_t		GetCountShaders() const;
		virtual const char*		GetShader( UInt32_t Index ) const;
		virtual const char**	GetShaders() const;

		// ShaderFactory
		ShaderFactory();
		~ShaderFactory();

	private:
		std::vector< const char* >									nameShaders;
		std::unordered_map< std::string, CreateShaderFn_t >			shaders;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_FACTORY_H

