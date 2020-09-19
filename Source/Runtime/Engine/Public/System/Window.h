// Copyright BSOD-Games, All Rights Reserved.
// Authors: Egor Pogulyaka (zombiHello)

#ifndef WINDOW_W
#define WINDOW_W

#include <string>

#include "Misc/EngineDefines.h"
#include "Misc/Types.h"
#include "System/Object.h"
#include "Event.h"

namespace le
{
	typedef void*			FWindowHandle;

	enum EStyleWindow
	{
		SW_None = 0,
		SW_Resizable = 1 << 0,
		SW_Decorated = 1 << 1,
		SW_Floating = 1 << 2,
		SW_Fullscreen = 1 << 3,

		SW_Default = SW_Decorated | SW_Resizable
	};

	enum EMessageBoxType
	{
		MBT_Info,
		MBT_Warning,
		MBT_Error
	};

	class Window : public Object
	{
	public:
		/* Constructor */
		Window();

		/* Destructor */
		~Window();

		/* Show message box */
		static void ShowMessageBox( const std::string& InMessage, EMessageBoxType InType );

		/* Open window */
		bool Open( const std::string& InTitle, uint32 InWidth, uint32 InHeight, EStyleWindow InStyleWindow = SW_Default );

		/* Close window */
		void Close();

		/* Poll event */
		bool PollEvent( SEvent& OutEvent );

		/* Is open */
		FORCEINLINE bool IsOpen() const
		{
			return handle;
		}

		/* Get handle window */
		FORCEINLINE FWindowHandle GetHandle() const
		{
			return handle;
		}

	private:
		struct SDL_Window*			window;
		struct SDL_Cursor*			cursor;
		struct SDL_SysWMinfo*		sysWMinfo;
		
		bool						isShowCursor;
		FWindowHandle				handle;
	};
}

#endif // !WINDOW_W
