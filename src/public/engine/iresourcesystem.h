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

	//---------------------------------------------------------------------//

	class IResourceSystem
	{
	public:
		virtual ~IResourceSystem() {}
		virtual void					RegisterLoader( IParserImage* ParserImage ) = 0;
		virtual void					RegisterLoader( IParserTexture* ParserTexture ) = 0;
		virtual void					RegisterLoader( IParserMaterial* ParserMaterial ) = 0;
		virtual void					RegisterLoader( IParserMesh* ParserMesh ) = 0;
		virtual void					RegisterLoader( IParserLevel* ParserLevel ) = 0;
		virtual void					RegisterLoader( IParserFont* ParserFont ) = 0;
		virtual void					RegisterLoader( IParserScript* ParserScript ) = 0;
		virtual void					RegisterLoader( IParserPhysicsModel* ParserPhysicsModel ) = 0;
		virtual void					RegisterLoader( IParserGPUProgram* ParserGPUProgram ) = 0;
		virtual void					RegisterLoader( IParserSoundBuffer* ParserSoundBuffer ) = 0;

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
