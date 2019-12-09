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
#include <vector>

#include "engine/lifeengine.h"
#include "materialsystem/imaterialsysteminternal.h"
#include "materialsystem/materialsystemfactory.h"

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

	class MaterialSystem : public IMaterialSystemInternal
	{
	public:
		// IMaterialSystemInternal
		virtual bool			Initialize( IEngine* Engine );

		// IMaterialSystem
		virtual bool			LoadShaderDLL( const char* FullPath );
		virtual void			UnloadShaderDLL( const char* FullPath );

		virtual IFactory*		GetFactory() const;

		// MaterialSystem
		MaterialSystem();
		~MaterialSystem();

		IShader*				FindShader( const char* Name ) const;

	private:
		// MaterialSystem
		void					UnloadShaderDLL( const ShaderDLLDescriptor& ShaderDLLDescriptor );

		MaterialSystemFactory					materialSystemFactory;
		std::vector<ShaderDLLDescriptor>		shaderLibs;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIALSYSTEM_H

