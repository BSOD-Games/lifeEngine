//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LIFEENGINE_H
#define LIFEENGINE_H

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;
	class IStudioRender;
	class IGame;
	class IShaderDLL;
	class IMaterialSystem;

	//---------------------------------------------------------------------//

	typedef		void						( *CriticalErrorFn_t )( const char* Message );
	typedef		void*						WindowHandle_t;
	typedef		IEngine*					( *LE_CreateEngineFn_t )( );
	typedef		void						( *LE_DeleteEngineFn_t )( IEngine* Engine );
	typedef		IStudioRender*				( *LE_CreateStudioRenderFn_t )( );
	typedef		void						( *LE_DeleteStudioRenderFn_t )( IStudioRender* StudioRender );
	typedef		IGame*						( *LE_CreateGameFn_t )( );
	typedef		void						( *LE_DeleteGameFn_t )( IGame* Game );
	typedef		void						( *LE_SetCriticalErrorFn_t )( CriticalErrorFn_t CriticalError );
	typedef		IShaderDLL*					( *LE_CreateShaderDLLFn_t )();
	typedef		void						( *LE_DeleteShaderDLLFn_t )( IShaderDLL* ShaderDLL );
	typedef		IMaterialSystem*			( *LE_CreateMaterialSystemFn_t )( );
	typedef		void						( *LE_DeleteMaterialSystemFn_t )( IMaterialSystem* MaterialSystem );

	//---------------------------------------------------------------------//

	static CriticalErrorFn_t			g_criticalError;

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define LIFEENGINE_VERSION_MAJOR		0
#define LIFEENGINE_VERSION_MINOR		1
#define LIFEENGINE_VERSION_PATCH		0

//---------------------------------------------------------------------//

#	if defined( _WIN32 ) || defined( _WIN64 )
#		define PLATFORM_WINDOWS
#	else
#		error Unknown platform
#	endif // _WIN32 или _WIN64

//---------------------------------------------------------------------//

#	if defined( PLATFORM_WINDOWS )
#		define LIFEENGINE_API				extern "C" __declspec( dllexport )
#	else
#		define LIFEENGINE_API
#	endif // LIFEENGINE_EXPORT

//---------------------------------------------------------------------//

#	define	TO_STRING( X ) #X

//---------------------------------------------------------------------//

#	define LIFEENGINE_ENGINE_API( EngineClass ) \
		namespace le { class IEngine; } \
		LIFEENGINE_API le::IEngine* LE_CreateEngine() { return new EngineClass(); } \
		LIFEENGINE_API void LE_DeleteEngine( le::IEngine* Object ) { delete static_cast<EngineClass*>( Object ); } \
		LIFEENGINE_API void LE_SetCriticalError( le::CriticalErrorFn_t CriticalError ) { le::g_criticalError = CriticalError; }

#	define LIFEENGINE_STUDIORENDER_API( StudioRenderClass ) \
		namespace le { class IStudioRender; } \
		LIFEENGINE_API le::IStudioRender* LE_CreateStudioRender() { return new StudioRenderClass(); } \
		LIFEENGINE_API void LE_DeleteStudioRender( le::IStudioRender* Object ) { delete static_cast<StudioRenderClass*>( Object ); } \
		LIFEENGINE_API void LE_SetCriticalError( le::CriticalErrorFn_t CriticalError ) { le::g_criticalError = CriticalError; }

#	define LIFEENGINE_GAME_API( GameClass ) \
		namespace le { class IGame; } \
		LIFEENGINE_API le::IGame* LE_CreateGame() { return new GameClass(); } \
		LIFEENGINE_API void LE_DeleteGame( le::IGame* Object ) { delete static_cast<GameClass*>( Object ); } \
		LIFEENGINE_API void LE_SetCriticalError( le::CriticalErrorFn_t CriticalError ) { le::g_criticalError = CriticalError; }

#	define LIFEENGINE_STDSHADERS_API( ShaderDLLClass ) \
		namespace le { class IShaderDLL; } \
		LIFEENGINE_API le::IShaderDLL* LE_CreateShaderDLL() { return new ShaderDLLClass(); } \
		LIFEENGINE_API void LE_DeleteShaderDLL( le::IShaderDLL* Object ) { delete static_cast<ShaderDLLClass*>( Object ); } \
		LIFEENGINE_API void LE_SetCriticalError( le::CriticalErrorFn_t CriticalError ) { le::g_criticalError = CriticalError; }

#	define LIFEENGINE_MATERIALSYSTEM_API( MaterialSystemClass ) \
		namespace le { class IMaterialSystem; } \
		LIFEENGINE_API le::IMaterialSystem* LE_CreateMaterialSystem() { return new MaterialSystemClass(); } \
		LIFEENGINE_API void LE_DeleteMaterialSystem( le::IMaterialSystem* Object ) { delete static_cast<MaterialSystemClass*>( Object ); } \
		LIFEENGINE_API void LE_SetCriticalError( le::CriticalErrorFn_t CriticalError ) { le::g_criticalError = CriticalError; }

//---------------------------------------------------------------------//

#	if defined ( LIFEENGINE_DEBUG )
#		define LIFEENGINE_ASSERT_MSG( X, File, Function, Line ) \
			if ( le::g_criticalError ) \
				le::g_criticalError( "*** LIFEENGINE_ASSERT ***" "\nFile: " File "\nFunction: " Function "\nLine: " TO_STRING( Line ) "\n\nAssertion failed: " #X )

#		define LIFEENGINE_ASSERT( X ) \
			if ( !( X ) ) \
			{ \
				LIFEENGINE_ASSERT_MSG( X, __FILE__, __FUNCTION__, __LINE__ ); \
			}		
#	else
#		define LIFEENGINE_ASSERT( X )
#	endif // LIFEENGINE_DEBUG

//---------------------------------------------------------------------//

#endif // !LIFEENGINE_H
