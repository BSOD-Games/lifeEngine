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

	class IConsoleSystem
	{
	public:
		virtual void		PrintInfo( const char* Message, ... ) = 0;
		virtual void		PrintWarning( const char* Message, ... ) = 0;
		virtual void		PrintError( const char* Message, ... ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ICONSOLESYSTEM_H

