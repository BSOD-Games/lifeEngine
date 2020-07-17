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
	class IParserFont;
	class IParserGPUProgram;
	class IParserImage;
	class IParserLevel;
	class IParserMaterial;
	class IParserMesh;
	class IParserPhysicsModel;
	class IParserScript;
	class IParserSoundBuffer;
	class IParserTexture;
	class ITexture;
	class IMaterial;
	class IMesh;
	class ILevel;
	class IFont;
	class IScript;
	class IPhysicsModel;
	class IGPUProgram;
	class ISoundBuffer;
	class IStreamSound;

	typedef IParserImage*			( *CreateParserImageFn_t )();
	typedef IParserTexture*			( *CreateParserTextureFn_t )( );
	typedef IParserMaterial*		( *CreateParserMaterialFn_t )( );
	typedef IParserMesh*			( *CreateParserMeshFn_t )( );
	typedef IParserLevel*			( *CreateParserLevelFn_t )( );
	typedef IParserFont*			( *CreateParserFontFn_t )( );
	typedef IParserScript*			( *CreateParserScriptFn_t )( );
	typedef IParserPhysicsModel*	( *CreateParserPhysicsModelFn_t )( );
	typedef IParserGPUProgram*		( *CreateParserGPUProgramFn_t )( );
	typedef IParserSoundBuffer*		( *CreateParserSoundBufferFn_t )( );

	//---------------------------------------------------------------------//

	class IResourceSystem
	{
	public:
		virtual ~IResourceSystem() {}
		virtual void					RegisterLoader( CreateParserImageFn_t CreateParserImage ) = 0;
		virtual void					RegisterLoader( CreateParserTextureFn_t CreateParserTexture ) = 0;
		virtual void					RegisterLoader( CreateParserMaterialFn_t CreateParserMaterial ) = 0;
		virtual void					RegisterLoader( CreateParserMeshFn_t CreateParserMesh ) = 0;
		virtual void					RegisterLoader( CreateParserLevelFn_t CreateParserLevel ) = 0;
		virtual void					RegisterLoader( CreateParserFontFn_t CreateParserFont ) = 0;
		virtual void					RegisterLoader( CreateParserScriptFn_t CreateParserScript ) = 0;
		virtual void					RegisterLoader( CreateParserPhysicsModelFn_t CreateParserPhysicsModel ) = 0;
		virtual void					RegisterLoader( CreateParserGPUProgramFn_t CreateParserGPUProgram ) = 0;
		virtual void					RegisterLoader( CreateParserSoundBufferFn_t CreateParserSoundBuffer ) = 0;

		virtual Image					LoadImage( const char* Path, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false ) = 0;
		virtual ITexture*				LoadTexture( const char* Name, const char* Path ) = 0;
		virtual IMaterial*				LoadMaterial( const char* Name, const char* Path ) = 0;
		virtual IMesh*					LoadMesh( const char* Name, const char* Path ) = 0;
		virtual ILevel*					LoadLevel( const char* Name, const char* Path, IFactory* GameFactory ) = 0;
		virtual IFont*					LoadFont( const char* Name, const char* Path ) = 0;
		virtual IScript*				LoadScript( const char* Name, const char* Path, UInt32_t CountFunctions = 0, ScriptDescriptor::Symbol* Functions = nullptr, UInt32_t CountVars = 0, ScriptDescriptor::Symbol* Vars = nullptr ) = 0;
		virtual IPhysicsModel*			LoadPhysicsModel( const char* Name, const char* Path ) = 0;
		virtual IGPUProgram*			LoadGPUProgram( const char* Name, const char* Path, UInt32_t Flags = 0, UInt32_t CountDefines = 0, const char** Defines = nullptr ) = 0;
		virtual ISoundBuffer*			LoadSoundBuffer( const char* Name, const char* Path ) = 0;
		virtual IStreamSound*			OpenStreamSound( const char* Path ) = 0;
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
		virtual void					UnloadSoundBuffer( const char* Name ) = 0;
		virtual void					UnloadTextures() = 0;
		virtual void					UnloadMaterials() = 0;
		virtual void					UnloadMeshes() = 0;
		virtual void					UnloadLevels() = 0;
		virtual void					UnloadFonts() = 0;
		virtual void					UnloadScripts() = 0;
		virtual void					UnloadPhysicsModels() = 0;
		virtual void					UnloadGPUPrograms() = 0;
		virtual void					UnloadSoundBuffers() = 0;
		virtual void					UnloadAll() = 0;

		virtual ITexture*				GetTexture( const char* Name ) const = 0;
		virtual IMaterial*				GetMaterial( const char* Name ) const = 0;
		virtual IMesh*					GetMesh( const char* Name ) const = 0;
		virtual ILevel*					GetLevel( const char* Name ) const = 0;
		virtual IFont*					GetFont( const char* Name ) const = 0;
		virtual IScript*				GetScript( const char* Name ) const = 0;
		virtual IPhysicsModel*			GetPhysicsModel( const char* Name ) const = 0;
		virtual IGPUProgram*			GetGPUProgram( const char* Name, UInt32_t Flags = 0 ) const = 0;
		virtual ISoundBuffer*			GetSoundBuffer( const char* Name ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IRESOURCESYSTEM_H
