//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LIGHTMAPPED_GENERIC_H
#define LIGHTMAPPED_GENERIC_H

#include "baseshader.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class LightmappedGeneric : public BaseShader
	{
	public:
		// IShader
		virtual bool					InitInstance( UInt32_t CountParams, IMaterialVar** MaterialVars );
		virtual void					OnDrawMesh( UInt32_t CountParams, IMaterialVar** MaterialVars, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );

		virtual const char* GetName() const;
		virtual const char* GetFallbackShader() const;

		// LightmappedGeneric
		LightmappedGeneric();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !UNLIT_GENERIC_H

