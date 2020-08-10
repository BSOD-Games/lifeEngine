//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUARESOURCESYSTEM_H
#define LUARESOURCESYSTEM_H

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAImage;
	class LUAMesh;
	class LUALevel;

	//---------------------------------------------------------------------//

	class LUAResourceSystem
	{
	public:
		static void					Register( lua_State* LuaVM );
		static LUAImage				LoadImage( const char* Path, bool IsFlipVertical = false, bool IsSwitchRedAndBlueChannels = false );
		static LUAMesh				LoadMesh( const char* Name, const char* Path );
		static LUALevel				LoadLevel( const char* Name, const char* Path );
		//static IFont*				LoadFont( const char* Name, const char* Path );
		//static IPhysicsModel*		LoadPhysicsModel( const char* Name, const char* Path );
		//static ISoundBuffer*		LoadSoundBuffer( const char* Name, const char* Path );
		//static IStreamSound*		OpenStreamSound( const char* Path );
		//static bool					OpenStreamSound( const char* Path, IStreamSound* StreamSound );
		static void					UnloadImage( LUAImage& Image );
		static void					UnloadMesh( const char* Name );
		static void					UnloadLevel( const char* Name );
		//static void					UnloadFont( const char* Name );
		//static void					UnloadPhysicsModel( const char* Name );
		//static void					UnloadSoundBuffer( const char* Name );
		//static void					UnloadTextures();
		//static void					UnloadMaterials();
		static void					UnloadMeshes();
		static void					UnloadLevels();
		//static void					UnloadFonts();
		//static void					UnloadPhysicsModels();
		//static void					UnloadSoundBuffers();
		static void					UnloadAll();

		static LUAMesh				GetMesh( const char* Name );
		static LUALevel				GetLevel( const char* Name );
		/*static IFont*				GetFont( const char* Name );
		static IPhysicsModel*		GetPhysicsModel( const char* Name );
		static ISoundBuffer*		GetSoundBuffer( const char* Name );*/
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUARESOURCESYSTEM_H
