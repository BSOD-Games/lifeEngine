//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CONSOLESYSTEM_H
#define CONSOLESYSTEM_H

#include <stdio.h>
#include <string>
#include <unordered_map>

#include "common/types.h"
#include "engine/iconsolesysteminternal.h"
#include "engine/consolesystemfactory.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ConsoleSystem : public IConsoleSystemInternal
	{
	public:
		friend void			CMD_Help( UInt32_t CountArguments, const char** Arguments );

		// IConsoleSystemInternal
		virtual void		Initialize( const char* LogFile );

		// IConsoleSystem
		virtual void		RegisterVar( IConVar* ConVar );
		virtual void		RegisterCommand( IConCmd* ConCmd );
		virtual void		UnregisterVar( const char* Name );
		virtual void		UnregisterCommand( const char* Name );
		virtual bool		Exec( const char* Command );
		virtual void		PrintInfo( const char* Message, ... );
		virtual void		PrintWarning( const char* Message, ... );
		virtual void		PrintError( const char* Message, ... );

		virtual IConVar*	GetVar( const char* Name ) const;
		virtual IConCmd*	GetCommand( const char* Name ) const;
		virtual IFactory*	GetFactory() const;

		// ConsoleSystem
		ConsoleSystem();
		~ConsoleSystem();

	private:
		FILE*												fileLog;

		ConsoleSystemFactory								consoleSystemFactory;
		std::unordered_map< std::string, IConVar* >			vars;
		std::unordered_map< std::string, IConCmd* >			commands;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !CONSOLESYSTEM_H
