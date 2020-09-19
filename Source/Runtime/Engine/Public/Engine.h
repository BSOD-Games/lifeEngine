// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "System/Object.h"

namespace le
{
	class Engine : public Object
	{
	public:
		/* Constructor */
		Engine();

		/* Destructor */
		~Engine();

		/* Initialize engine */
		bool Initialize( const std::string& InConfigPath, const std::string& InLogPath );
	};
}

#endif // !ENGINE_H
