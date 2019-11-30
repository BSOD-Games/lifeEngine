//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SETTINGS_CONTEXT_H
#define SETTINGS_CONTEXT_H

#include "common/types.h"
using namespace std;

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//

	struct SettingsContext
	{
		//----------------------------------------------------------------------//

		enum CONTEXT_ATTRIBUTE
		{
			CA_DEFAULT	= 0,	
			CA_CORE		= 1 << 0,
			CA_DEBUG	= 1 << 2
		};

		//----------------------------------------------------------------------//
		
		SettingsContext( UInt32_t RedBits = 8, UInt32_t GreenBits = 8, UInt32_t BlueBits = 8, UInt32_t AlphaBits = 8, UInt32_t DepthBits = 8, UInt32_t StencilBits = 0, UInt32_t MajorVersion = 1, UInt32_t MinorVersion = 1, UInt32_t AttributeFlags = CA_DEFAULT ) :
			redBits( RedBits ),
			greenBits( GreenBits ),
			blueBits( BlueBits ),
			alphaBits( AlphaBits ),
			depthBits( DepthBits ),
			stencilBits( StencilBits ),
			majorVersion( MajorVersion ),
			minorVersion( MinorVersion ),
			attributeFlags( AttributeFlags )
		{}

		UInt32_t			redBits;			
		UInt32_t			greenBits;			
		UInt32_t			blueBits;			
		UInt32_t			alphaBits;			
		UInt32_t			depthBits;			
		UInt32_t			stencilBits;		
		UInt32_t			majorVersion;		
		UInt32_t			minorVersion;		
		UInt32_t			attributeFlags;		
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !SETTINGS_CONTEXT_H