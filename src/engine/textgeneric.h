//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef TEXT_GENERIC_H
#define TEXT_GENERIC_H

#include "baseshader.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class TextGeneric : public BaseShader
	{
	public:
		// IShader
		virtual bool					InitInstance( UInt32_t CountParams, IShaderParameter** ShaderParameters );
		virtual void					OnDrawMesh( UInt32_t CountParams, IShaderParameter** ShaderParameters, const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );

		virtual const char* 			GetName() const;
		virtual const char* 			GetFallbackShader() const;

		// TextGeneric
		TextGeneric();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !TEXT_GENERIC_H

