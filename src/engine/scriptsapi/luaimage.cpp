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

#include "common/image.h"
#include "scriptsapi/luaimage.h"

// ------------------------------------------------------------------------------------ //
// Register image
// ------------------------------------------------------------------------------------ //
void le::LUAImage::Register( lua_State* LuaVM )
{
	if ( !LuaVM )		return;

	// Register image
	luabridge::getGlobalNamespace( LuaVM ).
		beginClass<Image>( "Image" ).
		addConstructor< void (*) () >().
		addData( "width", &Image::width, false ).
		addData( "height", &Image::height, false ).
		addData( "depth", &Image::depth, false ).
		addData( "pitch", &Image::pitch, false ).
		addData( "rMask", &Image::rMask, false ).
		addData( "gMask", &Image::gMask, false ).
		addData( "bMask", &Image::bMask, false ).
		addData( "aMask", &Image::aMask, false ).
		addFunction( "IsEmpty", &LUAImage::IsEmpty ).
		endClass();
}

// ------------------------------------------------------------------------------------ //
// Is empty
// ------------------------------------------------------------------------------------ //
bool le::LUAImage::IsEmpty( Image* Object )
{
	if ( !Object ) return false;
	return !Object->data;
}
