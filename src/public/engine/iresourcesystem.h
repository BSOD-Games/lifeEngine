//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IRESOURCESYSTEM_H
#define IRESOURCESYSTEM_H

#include "common/types.h"
#include "engine/scriptdescriptor.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Image;
	class IFactory;
	class ITexture;
	class IMaterial;
	class IMesh;
	class ILevel;
	class IFont;
	class IResourceSystem;
	class IMaterialSystem;
	class IScript;
	class IPhysicsModel;
	class IGPUProgram;

	typedef		void				( *LoadImageFn_t )( const char* Path, Image& Image, bool& IsError, bool IsFlipVertical, bool IsSwitchRedAndBlueChannels );
	typedef		ITexture*			( *LoadTextureFn_t )( const char* Path, IFactory* StudioRenderFactory );
	typedef		IMaterial*			( *LoadMaterialFn_t )( const char* Path, IResourceSystem* ResourceSystem, IMaterialSystem* MaterialSystem, IFactory* EngineFactory );
	typedef		IMesh*				( *LoadMeshFn_t )( const char* Path, IResourceSystem* ResourceSystem, IFactory* StudioRenderFactory );
	typedef		ILevel*				( *LoadLevelFn_t )( const char* Path, IFactory* GameFactory );
	typedef		IFont*				( *LoadFontFn_t )( const char* Path );
	typedef		IScript*			( *LoadScriptFn_t )( const char* Path, UInt32_t CountFunctions, ScriptDescriptor::Symbol* Functions, UInt32_t CountVars, ScriptDescriptor::Symbol* Vars, IFactory* ScriptSystemFactory );
	typedef		IPhysicsModel*		( *LoadPhysicsModelFn_t )( const char* Path, IFactory* PhysicsSystemFactory );
	typedef		IGPUProgram*		( *LoadGPUProgramFn_t )( const char* Directory, const char* Path, UInt32_t CountDefines, const char** Defines, IFactory* StudioRenderFactory );

	//---------------------------------------------------------------------//

	class IResourceSystem
	{
	public:
		virtual ~IResourceSystem() {}
		virtual void					RegisterLoader_Image( const char* Format, LoadImageFn_t LoadImage ) = 0;
		virtual void					RegisterLoader_Texture( const char* Format, LoadTextureFn_t LoadTexture ) = 0;
		virtual void					RegisterLoader_Material( const char* Format, LoadMaterialFn_t LoadMaterial ) = 0;
		virtual void					RegisterLoader_Mesh( const char* Format, LoadMeshFn_t LoadMesh ) = 0;
		virtual void					RegisterLoader_Level( const char* Format, LoadLevelFn_t LoadLevel ) = 0;
		virtual void					RegisterLoader_Font( const char* Format, LoadFontFn_t LoadFont ) = 0;
		virtual void					RegisterLoader_Script( const char* Format, LoadScriptFn_t LoadScript ) = 0;
		virtual void					RegisterLoader_PhysicsModel( const char* Format, LoadPhysicsModelFn_t LoadPhysicsModel ) = 0;
		virtual void					RegisterLoader_GPUProgram( const char* Format, LoadGPUProgramFn_t LoadGPUProgram ) = 0;
		virtual void					UnregisterLoader_Image( const char* Format ) = 0;
		virtual void					UnregisterLoader_Texture( const char* Format ) = 0;
		virtual void					UnregisterLoader_Material( const char* Format ) = 0;
		virtual void					UnregisterLoader_Mesh( const char* Format ) = 0;
		virtual void					UnregisterLoader_Level( const char* Format ) = 0;
		virtual void					UnregisterLoader_Font( const char* Format ) = 0;
		virtual void					UnregisterLoader_Script( const char* Format ) = 0;
		virtual void					UnregisterLoader_PhysicsModel( const char* Format ) = 0;
		virtual void					UnregisterLoader_GPUProgram( const char* Format ) = 0;

		virtual Image					LoadImage( const char* Path, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false ) = 0;
		virtual ITexture*				LoadTexture( const char* Name, const char* Path ) = 0;
		virtual IMaterial*				LoadMaterial( const char* Name, const char* Path ) = 0;
		virtual IMesh*					LoadMesh( const char* Name, const char* Path ) = 0;
		virtual ILevel*					LoadLevel( const char* Name, const char* Path, IFactory* GameFactory ) = 0;
		virtual IFont*					LoadFont( const char* Name, const char* Path ) = 0;
		virtual IScript*				LoadScript( const char* Name, const char* Path, UInt32_t CountFunctions = 0, ScriptDescriptor::Symbol* Functions = nullptr, UInt32_t CountVars = 0, ScriptDescriptor::Symbol* Vars = nullptr ) = 0;
		virtual IPhysicsModel*			LoadPhysicsModel( const char* Name, const char* Path ) = 0;
		virtual IGPUProgram*			LoadGPUProgram( const char* Name, const char* Path, UInt32_t Flags = 0, UInt32_t CountDefines = 0, const char** Defines = nullptr ) = 0;
		virtual void					UnloadImage( Image& Image ) = 0;
		virtual void					UnloadTexture( const char* Name ) = 0;
		virtual void					UnloadMaterial( const char* Name ) = 0;
		virtual void					UnloadMesh( const char* Name ) = 0;
		virtual void					UnloadLevel( const char* Name ) = 0;
		virtual void					UnloadFont( const char* Name ) = 0;
		virtual void					UnloadScript( const char* Name ) = 0;
		virtual void					UnloadPhysicsModel( const char* Name ) = 0;
		virtual void					UnloadGPUProgram( const char* Name, UInt32_t Flags ) = 0;
		virtual void					UnloadGPUProgram( const char* Name ) = 0;
		virtual void					UnloadTextures() = 0;
		virtual void					UnloadMaterials() = 0;
		virtual void					UnloadMeshes() = 0;
		virtual void					UnloadLevels() = 0;
		virtual void					UnloadFonts() = 0;
		virtual void					UnloadScripts() = 0;
		virtual void					UnloadPhysicsModels() = 0;		
		virtual void					UnloadGPUPrograms() = 0;
		virtual void					UnloadAll() = 0;

		virtual ITexture*				GetTexture( const char* Name ) const = 0;
		virtual IMaterial*				GetMaterial( const char* Name ) const = 0;
		virtual IMesh*					GetMesh( const char* Name ) const = 0;
		virtual ILevel*					GetLevel( const char* Name ) const = 0;
		virtual IFont*					GetFont( const char* Name ) const = 0;
		virtual IScript*				GetScript( const char* Name ) const = 0;
		virtual IPhysicsModel*			GetPhysicsModel( const char* Name ) const = 0;
		virtual IGPUProgram*			GetGPUProgram( const char* Name, UInt32_t Flags = 0 ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IRESOURCESYSTEM_H
