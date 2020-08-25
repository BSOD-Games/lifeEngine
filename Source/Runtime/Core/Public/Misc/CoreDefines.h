// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef COREDEFINES_H
#define COREDEFINES_H

#define LIFEENGINE_MAJOR			0
#define LIFEENGINE_MINOR			1
#define LIFEENGINE_PATH				0

// Platform specific
#if defined( _WIN32 ) || defined( _WIN64 )	
	#include "Platform/Windows/WindowsPlatform.h"
#else
	#error Unknown platform
#endif // _WIN32 or _WIN64

// Core macros
#define LIFEENGINE_DEPRECATED( Version, Message )			[ [ deprecated( Message " Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile." ) ] ]

// Macros for debug
#ifdef LIFEENGINE_DEBUG
	#define LIFEENGINE_ASSERT( X )			if ( !( X ) ) PLATFORM_BREAK()
#else
	#define LIFEENGINE_ASSERT( X )
#endif // LIFEENGINE_DEBUG

#endif // !COREDEFINES_H