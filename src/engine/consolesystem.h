//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef CONSOLESYSTEM_H
#define CONSOLESYSTEM_H

#include <stdio.h>
#include <string>
#include <unordered_map>

#include "engine/iconsolesysteminternal.h"
#include "engine/consolesystemfactory.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ConsoleSystem : public IConsoleSystemInternal
	{
	public:
		// IConsoleSystemInternal
		virtual void		Initialize();

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