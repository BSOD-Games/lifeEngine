// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef LAUNCHERPRIVATE_H
#define LAUNCHERPRIVATE_H

#include "Misc/Types.h"
#include "Misc/Path.h"
#include "Launcher.h"

namespace le
{
	class IGame;
}

/* Create game */
extern le::IGame* AppGameCreate();

/* Delete game */
extern void AppGameDelete( le::IGame*& InGame );

/* Get game manifest */
extern le::SGameManifest AppGetGameManifest();

/* Start engine loop */
int AppEngineLoop( int Argc, char** Argv );

#endif // !LAUNCHERPRIVATE_H
