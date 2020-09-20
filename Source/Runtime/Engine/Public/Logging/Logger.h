// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string>
#include <chrono>

#include "Misc/EngineDefines.h"
#include "Misc/Object.h"

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

	class Logger : public Object
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
		std::chrono::steady_clock::time_point		startLogging;
		FILE*										file;
	};	
}

#endif // !LOGGE