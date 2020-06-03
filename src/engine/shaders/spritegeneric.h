//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SPRITE_GENERIC_H
#define SPRITE_GENERIC_H

#include "engine/shaderdescriptor.h"
#include "baseshader.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class SpriteGeneric : public BaseShader
	{
	public:

		//---------------------------------------------------------------------//

		enum SHADER_FLAG
		{
			SF_NONE = 0,
			SF_BASETEXTURE = 1 << 0,
			SF_NORMAL_MAP = 1 << 1,
			SF_SPECULAR_MAP = 1 << 2
		};

		//---------------------------------------------------------------------//

		// IShader
		virtual bool				Initialize( UInt32_t CountParams, IShaderParameter** ShaderParameters );
		virtual void				OnDrawSprite( const Matrix4x4_t& Transformation, ICamera* Camera );
		virtual void				OnDrawStaticModel( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );

		virtual bool				IsEuqal( IShader* Shader ) const;
		virtual const char*			GetName() const;
		virtual const char*			GetFallbackShader() const;

		// SpriteGeneric
		SpriteGeneric();
		~SpriteGeneric();

		void						ClearParameters();
		static ShaderDescriptor		GetDescriptor();

	private:
		UInt32_t			flags;
		ITexture*			baseTexture;
		ITexture*			normalMap;
		ITexture*			specularMap;
		Vector3D_t			color;
		Vector4D_t			textureRect;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SPRITE_GENERIC_H

