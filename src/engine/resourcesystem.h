//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef RESOURCESYSTEM_H
#define RESOURCESYSTEM_H

#include <string>
#include <unordered_map>
#include <vector>

#include "engine/iresourcesysteminternal.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct GameInfo;
	class IFactory;
	class MaterialSystem;

	//---------------------------------------------------------------------//

	class ResourceSystem : public IResourceSystemInternal
	{
	public:
		// IResourceSystem
		virtual void					RegisterLoader( CreateParserImageFn_t CreateParserImage );
		virtual void					RegisterLoader( CreateParserTextureFn_t CreateParserTexture );
		virtual void					RegisterLoader( CreateParserMaterialFn_t CreateParserMaterial );
		virtual void					RegisterLoader( CreateParserMeshFn_t CreateParserMesh );
		virtual void					RegisterLoader( CreateParserLevelFn_t CreateParserLevel );
		virtual void					RegisterLoader( CreateParserFontFn_t CreateParserFont );
		virtual void					RegisterLoader( CreateParserPhysicsModelFn_t CreateParserPhysicsModel );
		virtual void					RegisterLoader( CreateParserGPUProgramFn_t CreateParserGPUProgram );
		virtual void					RegisterLoader( CreateParserSoundBufferFn_t CreateParserSoundBuffer );

		virtual Image					LoadImage( const char* Path, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false );
		virtual ITexture*				LoadTexture( const char* Name, const char* Path );
        virtual IMaterial*				LoadMaterial( const char* Name, const char* Path );
        virtual IMesh*					LoadMesh( const char* Name, const char* Path );
        virtual ILevel*					LoadLevel( const char* Name, const char* Path, IFactory* GameFactory );
		virtual IFont*					LoadFont( const char* Name, const char* Path );
		virtual IPhysicsModel*			LoadPhysicsModel( const char* Name, const char* Path );
		virtual IGPUProgram*			LoadGPUProgram( const char* Name, const char* Path, UInt32_t Flags = 0, UInt32_t CountDefines = 0, const char** Defines = nullptr );
		virtual ISoundBuffer*			LoadSoundBuffer( const char* Name, const char* Path );
		virtual IStreamSound*			OpenStreamSound( const char* Path );
		virtual bool					OpenStreamSound( const char* Path, IStreamSound* StreamSound );
		virtual void					UnloadImage( Image& Image );
		virtual void					UnloadTexture( const char* Name );
		virtual void					UnloadMaterial( const char* Name );
		virtual void					UnloadMesh( const char* Name );	
		virtual void					UnloadLevel( const char* Name );
		virtual void					UnloadFont( const char* Name );
		virtual void					UnloadPhysicsModel( const char* Name );
		virtual void					UnloadGPUProgram( const char* Name, UInt32_t Flags );
		virtual void					UnloadGPUProgram( const char* Name );
		virtual void					UnloadSoundBuffer( const char* Name );
		virtual void					UnloadTextures();
		virtual void					UnloadMaterials();
		virtual void					UnloadMeshes();
		virtual void					UnloadLevels();
		virtual void					UnloadFonts();			
		virtual void					UnloadPhysicsModels();	
		virtual void					UnloadGPUPrograms();
		virtual void					UnloadSoundBuffers();
		virtual void					UnloadAll();

		virtual ITexture*				GetTexture( const char* Name ) const;
		virtual IMaterial*				GetMaterial( const char* Name ) const;
		virtual IMesh*					GetMesh( const char* Name ) const;
		virtual ILevel*					GetLevel( const char* Name ) const;
		virtual IFont*					GetFont( const char* Name ) const;
		virtual IPhysicsModel*			GetPhysicsModel( const char* Name ) const;
		virtual IGPUProgram*			GetGPUProgram( const char* Name, UInt32_t Flags = 0 ) const;
		virtual ISoundBuffer*			GetSoundBuffer( const char* Name ) const;

		// IResourceSystemInternal
		virtual bool					Initialize( IEngine* Engine );
		virtual void					AddPath( const char* Path );
		virtual void					RemovePath( UInt32_t Index );
		virtual void					ClearPaths();

		virtual UInt32_t				GetCountPaths() const;
		virtual const char*				GetPath( UInt32_t Index ) const;

		// ResourceSystem
		ResourceSystem();
		~ResourceSystem();

	private:
		inline std::string						GetFormatFile( const std::string& Route );

		typedef			std::unordered_map< std::string, CreateParserImageFn_t >							LoaderImageMap_t;
		typedef			std::unordered_map< std::string, CreateParserTextureFn_t >							LoaderTextureMap_t;
		typedef			std::unordered_map< std::string, CreateParserMaterialFn_t >							LoaderMaterialMap_t;
		typedef			std::unordered_map< std::string, CreateParserMeshFn_t >								LoaderMeshMap_t;
		typedef			std::unordered_map< std::string, CreateParserLevelFn_t >							LoaderLevelMap_t;
		typedef			std::unordered_map< std::string, CreateParserFontFn_t >								LoaderFontMap_t;
		typedef			std::unordered_map< std::string, CreateParserPhysicsModelFn_t >						LoaderPhysicsModelMap_t;
		typedef			std::unordered_map< std::string, CreateParserGPUProgramFn_t >						LoaderGPUProgramMap_t;
		typedef			std::unordered_map< std::string, CreateParserSoundBufferFn_t >						LoaderSoundBufferMap_t;
		typedef			std::unordered_map< std::string, ITexture* >										TextureMap_t;
		typedef			std::unordered_map< std::string, IMaterial* >										MaterialMap_t;
		typedef			std::unordered_map< std::string, IMesh* >											MeshMap_t;
		typedef			std::unordered_map< std::string, ILevel* >											LevelMap_t;
		typedef			std::unordered_map< std::string, IFont* >											FontMap_t;
		typedef			std::unordered_map< std::string, IPhysicsModel* >									PhysicsModelMap_t;
		typedef			std::unordered_map< std::string, std::unordered_map< UInt32_t, IGPUProgram* > >		GPUProgramMap_t;
		typedef			std::unordered_map< std::string, ISoundBuffer* >									SoundBufferMap_t;

		IFactory*					studioRenderFactory;
		IFactory*					engineFactory;
		IFactory*					audioSystemFactory;

		std::vector< std::string >	paths;
		LoaderImageMap_t			loaderImages;
		LoaderTextureMap_t			loaderTextures;
		LoaderMaterialMap_t			loaderMaterials;
		LoaderMeshMap_t				loaderMeshes;
		LoaderLevelMap_t			loaderLevels;
		LoaderFontMap_t				loaderFonts;
		LoaderPhysicsModelMap_t		loaderPhysicsModels;
		LoaderGPUProgramMap_t		loaderGPUProgram;
		LoaderSoundBufferMap_t		loaderSoundBuffers;

		TextureMap_t				textures;
		MaterialMap_t				materials;
		MeshMap_t					meshes;
		LevelMap_t					levels;
		FontMap_t					fonts;	
		PhysicsModelMap_t			physicsModels;
		GPUProgramMap_t				gpuPrograms;
		SoundBufferMap_t			soundBuffers;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !RESOURCESYSTEM_H
