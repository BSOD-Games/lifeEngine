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
#include "engine/iconsolesysteminternal.h"

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
		virtual void		PrintInfo( const char* Message, ... );
		virtual void		PrintWarning( const char* Message, ... );
		virtual void		PrintError( const char* Message, ... );

		// ConsoleSystem
		ConsoleSystem();
		~ConsoleSystem();

	private:
		FILE*			fileLog;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !CONSOLESYSTEM_H