//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ICONSOLESYSTEM_H
#define ICONSOLESYSTEM_H

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IConVar;
	class IConCmd;

	//---------------------------------------------------------------------//

	class IConsoleSystem
	{
	public:
		virtual void		RegisterVar( IConVar* ConVar ) = 0;
		virtual void		RegisterCommand( IConCmd* ConCmd ) = 0;
		virtual void		UnregisterVar( const char* Name ) = 0;
		virtual void		UnregisterCommand( const char* Name ) = 0;
		virtual bool		Exec( const char* Command ) = 0;
		virtual void		PrintInfo( const char* Message, ... ) = 0;
		virtual void		PrintWarning( const char* Message, ... ) = 0;
		virtual void		PrintError( const char* Message, ... ) = 0;
		virtual IConVar*	GetVar( const char* Name ) const = 0;
		virtual IConCmd*	GetCommand( const char* Name ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define CONSOLE_SYSTEM_INTERFACE_VERSION "LE_ConsoleSystem001"

//---------------------------------------------------------------------//

#endif // !ICONSOLESYSTEM_H

