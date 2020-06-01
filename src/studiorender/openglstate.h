//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef OPENGL_STATE_H
#define OPENGL_STATE_H

#include <cstddef>
#include <unordered_map>

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum CULLFACE_TYPE;
	class Texture; 
	class GPUProgram;

	//---------------------------------------------------------------------//

	class OpenGLState
	{
	public:
		friend class StudioRender;

		static void				EnableDepthTest( bool Enable = true );
		static void				EnableDepthWrite( bool Enable = true );
		static void				EnableBlend( bool Enable = true );
		static void				EnableCullFace( bool Enable = true );
		static void				EnableStencilTest( bool Enable = true );

		static void				SetGPUProgram( GPUProgram* GPUProgram );
		static void				SetTextureLayer( UInt32_t Layer );
		static void				SetTexture( Texture* Texture );
		static void				SetCullFaceType( CULLFACE_TYPE CullFaceType );
		static void				SetColorMask( bool R, bool G, bool B, bool A );
		static void				SetStencilFunc( UInt32_t StencilFuncType, UInt32_t Ref, UInt32_t Mask );
		static void				SetBlendFunc( UInt32_t SFactor, UInt32_t DFactor );
		static void				SetBlendEquation( UInt32_t Mode );
		static void				SetStencilOpSeparate( UInt32_t Face, UInt32_t SFail, UInt32_t DpFail, UInt32_t DpPass );

	private:
		static void				Initialize();

		static bool												isDepthTest;
		static bool												isDepthWrite;
		static bool												isStencilTest;
		static bool												isBlend;
		static bool												isCullFace;

		static bool												colorMask[ 4 ];
		static CULLFACE_TYPE									cullFaceType;
		static UInt32_t 										stencilFuncType;
		static UInt32_t											stencilFunc_ref;
		static UInt32_t											stencilFunc_mask;
		static UInt32_t											blendFunc_sFactor;
		static UInt32_t											blendFunc_dFactor;
		static UInt32_t											blendEquation_mode;
		static UInt32_t											textureLayer;
		static GPUProgram*										bindedGPUProgram;

		static std::unordered_map< UInt32_t, Texture* >			bindedTextures;
	};	

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !OPENGL_STATE_H

