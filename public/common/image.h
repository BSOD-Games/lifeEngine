//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct Image
	{
		Image() :
			data( nullptr ),
			width( 0 ),
			height( 0 ),
			depth( 0 ),
			pitch( 0 ),
			rMask( 0 ),
			gMask( 0 ),
			bMask( 0 ),
			aMask( 0 )
		{}

		UInt8_t*		data;
		UInt32_t		width;
		UInt32_t		height;
		UInt32_t		depth;
		UInt32_t		pitch;
		UInt32_t		rMask;
		UInt32_t		gMask;
		UInt32_t		bMask;
		UInt32_t		aMask;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMAGE_H
