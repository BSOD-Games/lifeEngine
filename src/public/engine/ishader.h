//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISHADER_H
#define ISHADER_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ShaderParamInfo;
	class ICamera;
	class IShaderParameter;
	class ITexture;

	//---------------------------------------------------------------------//

	class IShader
	{
	public:
		virtual bool					InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters ) = 0;
		virtual void					OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr ) = 0;

		virtual const char*				GetName() const = 0;
		virtual const char*				GetFallbackShader() const = 0;
		virtual UInt32_t				GetCountParams() const = 0;
		virtual ShaderParamInfo*		GetParam( UInt32_t Index ) const = 0;
		virtual ShaderParamInfo*		GetParams() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISHADER_H

