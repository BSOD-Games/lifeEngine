// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Misc/Path.h"
#include "Misc/Object.h"

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
		bool Initialize( const Path& InLogPath );
	};
}

#endif // !ENGINE_H
