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
			major ( 0 ),
			minor( 0 ),
			path( 0 ),
			build( 0 ),
			date( __DATE__ ),
			time( __TIME__ )
		{}

		Version( UInt32_t Major, UInt32_t Minor, UInt32_t Path, UInt32_t Build ) :
			major( Major ),
			minor( Minor ),
			path( Path ),
			build( Build ),
			date( __DATE__ ),
			time( __TIME__ )
		{}

		UInt32_t		major;
		UInt32_t		minor;
		UInt32_t		path;
		UInt32_t		build;

		const char*		date;
		const char*		time;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !VERSION_H

