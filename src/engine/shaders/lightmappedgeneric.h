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
		virtual bool				Initialize( UInt32_t CountParams, IShaderParameter** ShaderParameters );
		virtual void				OnDrawStaticModel( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );

		virtual bool				IsEuqal( IShader* Shader ) const;
		virtual const char*			GetName() const;
		virtual const char*			GetFallbackShader() const;

		// LightmappedGeneric
		LightmappedGeneric();
		~LightmappedGeneric();

		void						ClearParameters();

	private:
		ITexture*			baseTexture;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LIGHTMAPPED_GENERIC_H

