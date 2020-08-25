// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string>

#include "Misc/CoreDefines.h"
#include "Templates/Singleton.h"

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

	class Logger : public TSingleton<Logger>
	{
	public:
		/* Constructor */
		Logger();

		/* Destructor */
		~Logger();

		/* Print message to log */
		void Logf( ELogType InLogType, const std::string& InCategory, const char* InMessage, ... );

		/* Close file */
		void CloseFile();

		/* Set path to log file */
		void SetFile( const std::string& InPath );

	private:
		FILE*		file;
	};	
}

#endif // !LOGGE