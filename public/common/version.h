//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef VERSION_H
#define VERSION_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Version
	{
		Version() :
			version( "" ),
			build( 0 ),
			date( __DATE__ ),
			time( __TIME__ )
		{}

		Version( const char* Version, UInt32_t Build ) :
			version( Version ),
			build( Build ),
			date( __DATE__ ),
			time( __TIME__ )
		{}

		const char*		version;
		UInt32_t		build;

		const char*		date;
		const char*		time;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !VERSION_H

