// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "Misc/Types.h"
#include "Misc/Path.h"

#include <string>

namespace le
{
	struct SGameManifest
	{
		std::string		windowName;
		uint32			windowWidth;
		uint32			windowHeight;
		Path			logPath;
		Path			rootPath;
	};
}

#define IMPLEMENT_INTERFACE_GAME( GameClass, WindowName, WindowWidth, WindowHeight, LogPath, RootPath ) \
	le::IGame* AppGameCreate() \
	{ return ( le::IGame* ) new GameClass(); } \
	\
	void AppGameDelete( le::IGame*& InGame ) \
	{ \
		delete static_cast< GameClass* >( InGame ); \
		InGame = nullptr; \
	} \
	\
	le::SGameManifest AppGetGameManifest() \
	{ return le::SGameManifest{ WindowName, WindowWidth, WindowHeight, LogPath, RootPath }; }

#endif // !LAUNCHER_H
