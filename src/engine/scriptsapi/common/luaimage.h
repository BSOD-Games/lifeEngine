//////////////////////////////////////////////////////////////////////////
//
//					*** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Engine repository:  	https://github.com/BSOD-Games/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LUAIMAGE_H
#define LUAIMAGE_H

#include "common/image.h"

//---------------------------------------------------------------------//

struct lua_State;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LUAImage
	{
	public:
		static void				Register( lua_State* LuaVM );

		LUAImage();
		LUAImage( const Image& Copy );
		
		bool					IsEmpty() const;
		UInt32_t				GetWidth() const;
		UInt32_t				GetHeight() const;
		UInt32_t				GetDepth() const;
		UInt32_t				GetPitch() const;
		UInt32_t				GetRMask() const;
		UInt32_t				GetGMask() const;
		UInt32_t				GetBMask() const;
		UInt32_t				GetAMask() const;
		inline const Image&		GetHandle() const		{ return image; }

	private:
		Image					image;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LUAIMAGE_H
