//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef API_CONSOLESYSTEM_H
#define API_CONSOLESYSTEM_H

//---------------------------------------------------------------------//

/* Print info in console */
extern void		ConsoleSystem_Info( const char* Format, ... );

/* Print warning in console */
extern void		ConsoleSystem_Warning( const char* Format, ... );

/* Print error in console */
extern void		ConsoleSystem_Error( const char* Format, ... );

/* Execute console command */
extern void		ConsoleSystem_Exec( const char* Command );

//---------------------------------------------------------------------//

#endif // !API_CONSOLESYSTEM_H
