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

#include "engine/shaderdescriptor.h"
#include "baseshader.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class TextGeneric : public BaseShader
	{
	public:
		// IShader
		virtual bool				Initialize( UInt32_t CountParams, IShaderParameter** ShaderParameters );
		virtual void				OnDrawText( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Glyph = nullptr );

		virtual bool				IsEuqal( IShader* Shader ) const;
		virtual const char*			GetName() const;
		virtual const char*			GetFallbackShader() const;

		// TextGeneric
		TextGeneric();
		~TextGeneric();

		void						ClearParameters();
		static ShaderDescriptor		GetDescriptor();

	private:
		Vector3D_t			color;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !TEXT_GENERIC_H
