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

#	if defined( _WIN32 ) || defined( _WIN64 )
#		define PLATFORM_WINDOWS
#	else
#		error Unknown platform
#	endif // _WIN32 или _WIN64

//---------------------------------------------------------------------//

#	if defined( LIFEENGINE_EXPORT ) && defined( PLATFORM_WINDOWS )
#		define LIFEENGINE_API				extern "C" __declspec( dllexport )
#	else
#		define LIFEENGINE_API
#	endif // LIFEENGINE_EXPORT

//---------------------------------------------------------------------//

#	if defined( LIFEENGINE_EXPORT )
#		define LIFEENGINE_ENGINE_API( EngineClass ) \
			namespace le { class IEngine; } \
			LIFEENGINE_API le::IEngine* LE_CreateEngine() { return new EngineClass(); } \
			LIFEENGINE_API void LE_DeleteEngine( le::IEngine* Object ) { delete static_cast<EngineClass*>( Object ); }

#		define LIFEENGINE_SUBSYSTEM_API( SubsystemClass ) \
			namespace le { class IEngineSubsystem; } \
			LIFEENGINE_API le::IEngineSubsystem* LE_CreateSubsystem() { return new SubsystemClass(); } \
			LIFEENGINE_API void LE_DeleteSubsystem( le::IEngineSubsystem* Object ) { delete static_cast<SubsystemClass*>( Object ); }

#		define LIFEENGINE_GAME_API( GameClass ) \
			namespace le { class IGame; } \
			LIFEENGINE_API le::IGame* LE_CreateGame() { return new GameClass(); } \
			LIFEENGINE_API void LE_DeleteGame( le::IGame* Object ) { delete static_cast<GameClass*>( Object ); }
#	endif // LIFEENGINE_EXPORT

//---------------------------------------------------------------------//

#	if defined ( LIFEENGINE_DEBUG )
#		define LIFEENGINE_ASSERT( X, MSG ) \
			if ( !( X ) ) \
			{ \
				std::cout << "\n*** LIFEENGINE_ASSERT ***\nFunction: " << __FUNCTION__ << "\nLine: " << __LINE__ << "\n\nMessage: " << MSG; \
				abort(); \
			}		
#	else
#		define LIFEENGINE_ASSERT( X, MSG )
#	endif // LIFEENGINE_DEBUG

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;
	class IEngineSubsystem;
	class IGame;

	//---------------------------------------------------------------------//

	typedef		void*				WindowHandle_t;
	typedef		IEngine*			( *LE_CreateEngineFn_t )( );											
	typedef		void				( *LE_DeleteEngineFn_t )( IEngine* Engine );							
	typedef		IEngineSubsystem*	( *LE_CreateSubsystemFn_t )( );									
	typedef		void				( *LE_DeleteSubsystemFn_t )( IEngineSubsystem* EngineSubsystem );
	typedef		IGame*				( *LE_CreateGameFn_t )( );
	typedef		void				( *LE_DeleteGameFn_t )( IGame* Game );

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LIFEENGINE_H
