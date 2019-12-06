//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIALSYSTEM_H
#define MATERIALSYSTEM_H

#include <string>
#include <unordered_map>

#include "engine/lifeengine.h"
#include "materialsystem/imaterialsysteminternal.h"

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

		void*						handle;
		IShaderDLL*					shaderDLL;
		LE_CreateShaderDLLFn_t		LE_CreateShaderDLL;
		LE_DeleteShaderDLLFn_t		LE_DeleteShaderDLL;
		LE_SetCriticalErrorFn_t		LE_SetCriticalError;
	};

	//---------------------------------------------------------------------//

	class MaterialSystem : public IMaterialSystemInternal
	{
	public:
		// IMaterialSystemInternal
		virtual bool			Initialize( IEngine* Engine );

		// IMaterialSystem
		virtual bool			LoadShaderDLL( const char* FullPath );
		virtual void			UnloadShaderDLL( const char* FullPath );

		// MaterialSystem
		MaterialSystem();
		~MaterialSystem();

	private:
		typedef		std::unordered_map< std::string, ShaderDLLDescriptor >			ShaderDLLsMap_t;
		ShaderDLLsMap_t			shaderDLLs;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIALSYSTEM_H

