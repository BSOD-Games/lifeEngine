//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef UNLIT_GENERIC_H
#define UNLIT_GENERIC_H

#include "baseshader.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class UnlitGeneric : public BaseShader
	{
	public:

		//---------------------------------------------------------------------//

		enum SHADER_FLAG
		{
			SF_NONE = 0,
			SF_NORMAL_MAP = 1 << 0,
			SF_SPECULAR_MAP = 1 << 1
		};

		//---------------------------------------------------------------------//

		// IShader
		virtual bool				Initialize( UInt32_t CountParams, IShaderParameter** ShaderParameters );
		virtual void				OnDrawText( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Glyph = nullptr );
		virtual void				OnDrawSprite( const Matrix4x4_t& Transformation, ICamera* Camera );
		virtual void				OnDrawStaticModel( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );

		virtual bool				IsEuqal( IShader* Shader ) const;
		virtual const char*			GetName() const;
		virtual const char*			GetFallbackShader() const;

		// UnlitGeneric
		UnlitGeneric();
		~UnlitGeneric();

		void						ClearParameters();

	private:
		UInt32_t			flags;
		ITexture*			baseTexture;
		ITexture*			normalMap;
		ITexture*			specularMap;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !UNLIT_GENERIC_H

