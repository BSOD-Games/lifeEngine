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

#include "engine/shaderdescriptor.h"
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
			SF_BASETEXTURE = 1 << 0,
			SF_NORMAL_MAP = 1 << 1,
			SF_SPECULAR_MAP = 1 << 2,
			SF_ALPHA_MAP = 1 << 3
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
		static ShaderDescriptor		GetDescriptor();

	private:
		UInt32_t			flags;
		ITexture*			baseTexture;
		ITexture*			normalMap;
		ITexture*			specularMap;
		ITexture*			alphamap;
		Vector3D_t			color;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !UNLIT_GENERIC_H

