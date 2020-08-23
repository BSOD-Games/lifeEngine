// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef WINDOW_H
#define WINDOW_H

#include <string>

#include "Misc/CoreDefines.h"
#include "Misc/Types.h"

namespace le
{
	enum EStyleWindow
	{
		SW_None = 0,
		SW_Resizable = 1 << 0,
		SW_Decorated = 1 << 1,
		SW_Floating = 1 << 2,
		SW_Fullscreen = 1 << 3,

		SW_Default = SW_Decorated | SW_Resizable
	};

	class Window
	{
	public:
		/* Constructor */
		CORE_API Window();

		/* Destructor */
		CORE_API ~Window();

		/* Open window */
		CORE_API bool Open( const std::string& InTitle, uint32 InWidth, uint32 InHeight, EStyleWindow InStyleWindow );

		/* Close window */
		CORE_API void Close();

		/* Set window handle */
		CORE_API void SetHandle( WindowHandle_t InWindowHandle );

		/* Get window handle */
		CORE_API WindowHandle_t GetHandle() const;

	private:
		WindowHandle_t				handle;
	};
}

#endif // !WINDOW_H
