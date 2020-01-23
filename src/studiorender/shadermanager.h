//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "engine/lifeengine.h"
#include "studiorender/ishadermanager.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IShaderDLL;

	//---------------------------------------------------------------------//

	struct ShaderDLLDescriptor
	{
		ShaderDLLDescriptor() :
			handle( nullptr ),
			shaderDLL( nullptr ),
			LE_CreateShaderDLL( nullptr ),
			LE_DeleteShaderDLL( nullptr ),
			LE_SetCriticalError( nullptr )
		{}

		std::string										fileName;
		void*											handle;
		IShaderDLL*										shaderDLL;
		std::unordered_map< std::string, IShader* >		shaders;

		LE_CreateShaderDLLFn_t							LE_CreateShaderDLL;
		LE_DeleteShaderDLLFn_t							LE_DeleteShaderDLL;
		LE_SetCriticalErrorFn_t							LE_SetCriticalError;
	};

	//---------------------------------------------------------------------//

	class ShaderManager : public IShaderManager
	{
	public:
		// IShaderSystem
		virtual bool			LoadShaderDLL( const char* FullPath );
		virtual void			UnloadShaderDLL( const char* FullPath );
		
		// ShaderSystem
		ShaderManager();
		~ShaderManager();

		IShader*				FindShader( const char* ShaderName ) const;

	private:
		void					UnloadShaderDLL( const ShaderDLLDescriptor& ShaderDLLDescriptor );

		std::vector< ShaderDLLDescriptor >		shaderLibs;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADERMANAGER_H

