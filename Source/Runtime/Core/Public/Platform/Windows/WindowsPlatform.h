// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef WINDOWSPLATFORM_H
#define WINDOWSPLATFORM_H

#define PLATFORM_WINDOWS

// Is debug?
#ifdef _DEBUG
	#define LIFEENGINE_DEBUG									/* Macro for debug engine */
	#define PLATFORM_BREAK()					( __nop(), __debugbreak() )
#else
	#define PLATFORM_BREAK()
#endif // _DEBUG

// Function type macros
#define VARARGS				__cdecl								/* Functions with variable arguments */
#define CDECL				__cdecl								/* Standard C function */
#define STDCALL				__stdcall							/* Standard calling convention */
#define FORCEINLINE			__forceinline						/* Force code to be inline */
#define FORCENOINLINE		__declspec( noinline )				/* Force code to NOT be inline */

#endif // !WINDOWSPLATFORM_H