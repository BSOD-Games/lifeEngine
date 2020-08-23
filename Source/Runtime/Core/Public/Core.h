// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef CORE_H
#define CORE_H

#include <string>

#include "Misc/CoreDefines.h"
#include "Logging/Logger.h"
#include "Window.h"

namespace le
{
	class Core
	{
	public:
		/* Constructor Core */
		CORE_API Core();

		/* Destructor Core */
		CORE_API ~Core();

		/* Initialize engine */
		CORE_API bool Initialize( const std::string& InConfigPath, const std::string& InLogPath );

		/* Get window */
		FORCEINLINE Window& GetWindow()
		{
			return window;
		}

		/* Get logger */
		FORCEINLINE Logger& GetLogger()
		{
			return logger;
		}

	private:
		Logger			logger;
		Window			window;
	};
}

#endif // !CORE_H
