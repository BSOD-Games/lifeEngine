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
		virtual bool					InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters );
		virtual void					OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );

		virtual const char* GetName() const;
		virtual const char* GetFallbackShader() const;

		// LightmappedGeneric
		LightmappedGeneric();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !UNLIT_GENERIC_H

