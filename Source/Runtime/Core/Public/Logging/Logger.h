// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string>

#include "Misc/CoreDefines.h"

namespace le
{
	enum ELogType
	{
		LT_Debug,
		LT_Info,
		LT_Warning,
		LT_Error,
		LT_Fail
	};

	class Logger
	{
	public:
		/* Constructor */
		CORE_API Logger();

		/* Destructor */
		CORE_API ~Logger();

		/* Print message to log */
		CORE_API void Logf( ELogType InLogType, const std::string& InCategory, const char* InMessage, ... );

		/* Close file */
		CORE_API void CloseFile();

		/* Set path to log file */
		CORE_API void SetFile( const std::string& InPath );

	private:
		FILE*		file;
	};	
}

#endif // !LOGGE