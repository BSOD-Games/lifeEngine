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

	//---------------------------------------------------------------------//

	class ResourceSystem : public IResourceSystemInternal
	{
	public:
		// IResourceSystem
		virtual void					RegisterLoader_Image( const char* Format, LoadImageFn_t LoadImage );
		virtual void					RegisterLoader_Texture( const char* Format, LoadTextureFn_t LoadTexture );
		virtual void					RegisterLoader_Material( const char* Format, LoadMaterialFn_t LoadMaterial );
		virtual void					RegisterLoader_Mesh( const char* Format, LoadMeshFn_t LoadMesh );
		virtual void					RegisterLoader_Level( const char* Format, LoadLevelFn_t LoadLevel );
		virtual void					RegisterLoader_Font( const char* Format, LoadFontFn_t LoadFont );
		virtual void					RegisterLoader_Script( const char* Format, LoadScriptFn_t LoadScript );
		virtual void					RegisterLoader_PhysicsModel( const char* Format, LoadPhysicsModelFn_t LoadPhysicsModel );
		virtual void					UnregisterLoader_Image( const char* Format );
		virtual void					UnregisterLoader_Texture( const char* Format );
		virtual void					UnregisterLoader_Material( const char* Format );
		virtual void					UnregisterLoader_Mesh( const char* Format );
		virtual void					UnregisterLoader_Level( const char* Format );
		virtual void					UnregisterLoader_Font( const char* Format );
		virtual void					UnregisterLoader_Script( const char* Format );
		virtual void					UnregisterLoader_PhysicsModel( const char* Format );

		virtual Image					LoadImage( const char* Path, bool& IsError, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false );
		virtual ITexture*				LoadTexture( const char* Name, const char* Path );
        virtual IMaterial*				LoadMaterial( const char* Name, const char* Path );
        virtual IMesh*					LoadMesh( const char* Name, const char* Path );
        virtual ILevel*					LoadLevel( const char* Name, const char* Path, IFactory* GameFactory );
		virtual IFont*					LoadFont( const char* Name, const char* Path );
		virtual IScript*				LoadScript( const char* Name, const char* Path, UInt32_t CountFunctions = 0, ScriptDescriptor::Symbol* Functions = nullptr, UInt32_t CountVars = 0, ScriptDescriptor::Symbol* Vars = nullptr );
		virtual IPhysicsModel*			LoadPhysicsModel( const char* Name, const char* Path );
		virtual void					UnloadImage( Image& Image );
		virtual void					UnloadTexture( const char* Name );
		virtual void					UnloadMaterial( const char* Name );
		virtual void					UnloadMesh( const char* Name );	
		virtual void					UnloadLevel( const char* Name );
		virtual void					UnloadFont( const char* Name );
		virtual void					UnloadScript( const char* Name );
		virtual void					UnloadPhysicsModel( const char* Name );
		virtual void					UnloadTextures();
		virtual void					UnloadMaterials();
		virtual void					UnloadMeshes();
		virtual void					UnloadLevels();
		virtual void					UnloadFonts();			
		virtual void					UnloadScripts();
		virtual void					UnloadPhysicsModels();
		virtual void					UnloadAll();

		virtual ITexture*				GetTexture( const char* Name ) const;
		virtual IMaterial*				GetMaterial( const char* Name ) const;
		virtual IMesh*					GetMesh( const char* Name ) const;
		virtual ILevel*					GetLevel( const char* Name ) const;
		virtual IFont*					GetFont( const char* Name ) const;
		virtual IScript*				GetScript( const char* Name ) const;
		virtual IPhysicsModel*			GetPhysicsModel( const char* Name ) const;

		// IResourceSystemInternal
		virtual bool					Initialize( IEngine* Engine );
		virtual void					AddPath( const char* Path );
		virtual void					RemovePath( UInt32_t Index );
		virtual void					ClearPaths();

		// ResourceSystem
		ResourceSystem();
		~ResourceSystem();

	private:
		inline std::string						GetFormatFile( const std::string& Route );

		typedef			std::unordered_map< std::string, LoadImageFn_t >			LoaderImageMap_t;
		typedef			std::unordered_map< std::string, LoadTextureFn_t >			LoaderTextureMap_t;
		typedef			std::unordered_map< std::string, LoadMaterialFn_t >			LoaderMaterialMap_t;
		typedef			std::unordered_map< std::string, LoadMeshFn_t >				LoaderMeshMap_t;
		typedef			std::unordered_map< std::string, LoadLevelFn_t >			LoaderLevelMap_t;
		typedef			std::unordered_map< std::string, LoadFontFn_t >				LoaderFontMap_t;
		typedef			std::unordered_map< std::string, LoadScriptFn_t >			LoaderScriptMap_t;
		typedef			std::unordered_map< std::string, LoadPhysicsModelFn_t >		LoaderPhysicsModelMap_t;
		typedef			std::unordered_map< std::string, ITexture* >				TextureMap_t;
		typedef			std::unordered_map< std::string, IMaterial* >				MaterialMap_t;
		typedef			std::unordered_map< std::string, IMesh* >					MeshMap_t;
		typedef			std::unordered_map< std::string, ILevel* >					LevelMap_t;
		typedef			std::unordered_map< std::string, IFont* >					FontMap_t;
		typedef			std::unordered_map< std::string, IScript* >					ScriptMap_t;
		typedef			std::unordered_map< std::string, IPhysicsModel* >			PhysicsModelMap_t;

		IFactory*					studioRenderFactory;
		IFactory*					scriptSystemFactory;
        IMaterialManager*           materialManager;

		std::vector< std::string >	paths;
		LoaderImageMap_t			loaderImages;
		LoaderTextureMap_t			loaderTextures;
		LoaderMaterialMap_t			loaderMaterials;
		LoaderMeshMap_t				loaderMeshes;
		LoaderLevelMap_t			loaderLevels;
		LoaderFontMap_t				loaderFonts;
		LoaderScriptMap_t			loaderScripts;
		LoaderPhysicsModelMap_t		loaderPhysicsModels;

		TextureMap_t				textures;
		MaterialMap_t				materials;
		MeshMap_t					meshes;
		LevelMap_t					levels;
		FontMap_t					fonts;	
		ScriptMap_t					scripts;
		PhysicsModelMap_t			physicsModels;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !RESOURCESYSTEM_H
