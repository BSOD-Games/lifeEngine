//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIORENDER_SAMPLER_H
#define STUDIORENDER_SAMPLER_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum SAMPLER_FILTER
	{
		SF_NEAREST,
		SF_LINEAR,
		SF_NEAREST_MIPMAP_NEAREST,
		SF_LINEAR_MIPMAP_NEAREST,
		SF_NEAREST_MIPMAP_LINEAR,
		SF_LINEAR_MIPMAP_LINEAR
	};

	//---------------------------------------------------------------------//

	enum SAMPLER_ADDRESS_MODE
	{
		SAM_REPEAT,
		SAM_MIRROR,
		SAM_CLAMP,
		SAM_BORDER
	};

	//---------------------------------------------------------------------//

	struct StudioRenderSampler
	{
		StudioRenderSampler() :
			minFilter( SF_NEAREST ),
			magFilter( SF_NEAREST ),
			addressU( SAM_REPEAT ),
			addressV( SAM_REPEAT ),
			addressW( SAM_REPEAT ),
			minLod( 0 ),
			maxLod( UINT32_MAX )
		{
			memset( borderColor, 0, 4 * sizeof( float ) );
		}

		SAMPLER_FILTER				minFilter;
		SAMPLER_FILTER				magFilter;
		SAMPLER_ADDRESS_MODE		addressU;
		SAMPLER_ADDRESS_MODE		addressV;
		SAMPLER_ADDRESS_MODE		addressW;
		float						borderColor[ 4 ];
		UInt32_t					minLod;
		UInt32_t					maxLod;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STUDIORENDER_SAMPLER_H
