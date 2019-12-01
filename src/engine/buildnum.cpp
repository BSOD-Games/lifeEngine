//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include "buildnum.h"

//---------------------------------------------------------------------//

class BuildNumber
{
public:
	BuildNumber()	{ ComputeBuildNumber(); }
	le::UInt32_t		GetBuildNumber() { return buildNumber; }

private:
	void		ComputeBuildNumber()
	{
		const char* date = __DATE__;

		const char* month[ 12 ] =
		{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

		const char		month_days[ 12 ] =
		{ 31,    28,    31,    30,    31,    30,    31,    31,    30,    31,    30,    31 };

		buildNumber = 0;
		int				months = 0;
		int				days = 0;
		int				years = 0;

		for ( months = 0; months < 11; ++months )
		{
			if ( strncmp( &date[ 0 ], month[ months ], 3 ) == 0 )
				break;

			days += month_days[ months ];
		}

		days += atoi( &date[ 4 ] ) - 1;
		years = atoi( &date[ 7 ] ) - 1900;

		buildNumber = days + static_cast< int >( ( years - 1 ) * 365.25f );

		if ( ( years % 4 == 0 ) && months > 1 )
			++buildNumber;

		buildNumber -= 42318; // Dec 12 2016 (начало разработки lifeEngine)
	}

	le::UInt32_t		buildNumber; 
};

//---------------------------------------------------------------------//

static BuildNumber				buildNumber;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	// ------------------------------------------------------------------------------------ //
	// Получить номер сборки
	// ------------------------------------------------------------------------------------ //
	UInt32_t Engine_BuildNumber()
	{
		return buildNumber.GetBuildNumber();
	}

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//