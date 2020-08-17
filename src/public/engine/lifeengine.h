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
    class IPhysicsSystem;
    class IAudioSystem;
    class IGameMode;

    //---------------------------------------------------------------------//

    typedef		void						( *CriticalErrorFn_t )( const char* Message );
    typedef		void*						WindowHandle_t;
    typedef		IEngine*					( *LE_CreateEngineFn_t )();
    typedef		void						( *LE_DeleteEngineFn_t )( IEngine* Engine );
    typedef		IStudioRender*				( *LE_CreateStudioRenderFn_t )();
    typedef		void						( *LE_DeleteStudioRenderFn_t )( IStudioRender* StudioRender );
    typedef		void						( *LE_SetCriticalErrorFn_t )( CriticalErrorFn_t CriticalError );
    typedef     IPhysicsSystem*             ( *LE_CreatePhysicsSystemFn_t )();
    typedef     void                        ( *LE_DeletePhysicsSystemFn_t )( IPhysicsSystem* PhysicsSystem );
    typedef     IAudioSystem*               ( *LE_CreateAudioSystemFn_t )();
    typedef     void                        ( *LE_DeleteAudioSystemFn_t )( IAudioSystem* AudioSystem );
    typedef     IGameMode*                  ( *LE_CreateGameModeFn_t )();
    typedef     void                        ( *LE_DeleteGameModeFn_t )( IGameMode* GameMode );

    //---------------------------------------------------------------------//

    static CriticalErrorFn_t			g_criticalError;

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#	if defined( _WIN32 ) || defined( _WIN64 )
#		define PLATFORM_WINDOWS
#   elif defined( __linux__ )
#       define PLATFORM_LINUX
#	else
#		error Unknown platform
#	endif // _WIN32 или _WIN64

//---------------------------------------------------------------------//

#	if defined( PLATFORM_WINDOWS )
#		define LIFEENGINE_API				extern "C" __declspec( dllexport )
#   elif defined( PLATFORM_LINUX )
#       define LIFEENGINE_API               extern "C"
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

#	define LIFEENGINE_PHYSICSSYSTEM_API( PhysicsSystemClass ) \
    namespace le { class IPhysicsSystem; } \
    LIFEENGINE_API le::IPhysicsSystem* LE_CreatePhysicsSystem() { return new PhysicsSystemClass(); } \
    LIFEENGINE_API void LE_DeletePhysicsSystem( le::IPhysicsSystem* Object ) { delete static_cast<PhysicsSystemClass*>( Object ); } \
    LIFEENGINE_API void LE_SetCriticalError( le::CriticalErrorFn_t CriticalError ) { le::g_criticalError = CriticalError; }

#	define LIFEENGINE_AUDIOSYSTEM_API( AudioSystemClass ) \
    namespace le { class IAudioSystem; } \
    LIFEENGINE_API le::IAudioSystem* LE_CreateAudioSystem() { return new AudioSystemClass(); } \
    LIFEENGINE_API void LE_DeleteAudioSystem( le::IAudioSystem* Object ) { delete static_cast<AudioSystemClass*>( Object ); } \
    LIFEENGINE_API void LE_SetCriticalError( le::CriticalErrorFn_t CriticalError ) { le::g_criticalError = CriticalError; }

#ifdef LIFEENGINE_EDITOR
#	    define LIFEENGINE_GAMEMODE_API( GameModeClass ) \
        namespace le { class IGameMode; } \
        LIFEENGINE_API le::IGameMode* LE_CreateGameMode() { return new GameModeClass(); } \
        LIFEENGINE_API void LE_DeleteGameMode( le::IGameMode* Object ) { delete static_cast<GameModeClass*>( Object ); } \
        LIFEENGINE_API void LE_SetCriticalError( le::CriticalErrorFn_t CriticalError ) { le::g_criticalError = CriticalError; }
#else
#       define LIFEENGINE_GAMEMODE_API( GameModeClass )
#endif // LIFEENGINE_EDITOR 

//---------------------------------------------------------------------//

#	if defined ( LIFEENGINE_DEBUG )
#		define LIFEENGINE_ASSERT_MSG( X, File, Line ) \
    if ( le::g_criticalError ) \
    le::g_criticalError( "*** LIFEENGINE_ASSERT ***" "\nFile: " File "\nLine: " TO_STRING( Line ) "\n\nAssertion failed: " #X )

#		define LIFEENGINE_ASSERT( X ) \
    if ( !( X ) ) \
{ \
    LIFEENGINE_ASSERT_MSG( X, __FILE__, __LINE__ ); \
    }
#	else
#		define LIFEENGINE_ASSERT( X )
#	endif // LIFEENGINE_DEBUG

//---------------------------------------------------------------------//

#endif // !LIFEENGINE_H
