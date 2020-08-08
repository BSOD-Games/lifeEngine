//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "global.h"
#include "scriptsapi/common/luaimage.h"

// ------------------------------------------------------------------------------------ //
// Register image in LUA
// ------------------------------------------------------------------------------------ //
void le::LUAImage::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Registern input system
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<LUAImage>( "Image" ).
		addConstructor< void (*) () >().
		addFunction( "IsEmpty", &LUAImage::IsEmpty ).
		addFunction( "GetWidth", &LUAImage::GetWidth ).
		addFunction( "GetHeight", &LUAImage::GetHeight ).
		addFunction( "GetDepth", &LUAImage::GetDepth ).
		addFunction( "GetPitch", &LUAImage::GetPitch ).
		addFunction( "GetRMask", &LUAImage::GetRMask ).
		addFunction( "GetGMask", &LUAImage::GetGMask ).
		addFunction( "GetBMask", &LUAImage::GetBMask ).
		addFunction( "GetAMask", &LUAImage::GetAMask ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAImage::LUAImage()
{}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::LUAImage::LUAImage( const Image& Copy ) :
	image( Copy )
{}

// ------------------------------------------------------------------------------------ //
// Is empty
// ------------------------------------------------------------------------------------ //
bool le::LUAImage::IsEmpty() const
{
	return !image.data;
}

// ------------------------------------------------------------------------------------ //
// Get width
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAImage::GetWidth() const
{
	return image.width;
}

// ------------------------------------------------------------------------------------ //
// Get height
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAImage::GetHeight() const
{
	return image.height;
}

// ------------------------------------------------------------------------------------ //
// Get depth
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAImage::GetDepth() const
{
	return image.depth;
}

// ------------------------------------------------------------------------------------ //
// Get pitch
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAImage::GetPitch() const
{
	return image.pitch;
}

// ------------------------------------------------------------------------------------ //
// Get rmask
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAImage::GetRMask() const
{
	return image.rMask;
}

// ------------------------------------------------------------------------------------ //
// Get gmask
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAImage::GetGMask() const
{
	return image.gMask;
}

// ------------------------------------------------------------------------------------ //
// Get bmask
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAImage::GetBMask() const
{
	return image.bMask;
}

// ------------------------------------------------------------------------------------ //
// Get amask
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::LUAImage::GetAMask() const
{
	return image.aMask;
}
