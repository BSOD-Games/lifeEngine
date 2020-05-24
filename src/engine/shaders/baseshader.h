//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef BASE_SHADER_H
#define BASE_SHADER_H

#include <unordered_map>
#include <vector>
#include <string>

#include "common/shaderparaminfo.h"
#include "engine/ishader.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IGPUProgram;

	//---------------------------------------------------------------------//

	class BaseShader : public IShader
	{
	public:
		// IShader
		virtual void				OnDrawText( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Glyph = nullptr );
		virtual void				OnDrawSprite( const Matrix4x4_t& Transformation, ICamera* Camera );
		virtual void				OnDrawStaticModel( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap = nullptr );

		// BaseShader
		BaseShader();
		virtual ~BaseShader();

	protected:
		bool								LoadShader( const std::string& Name, const std::string& Path, const std::vector< const char* >& Defines, UInt32_t Flags = 0 );

		IGPUProgram*						gpuProgram;

	private:
		std::string				nameShader;
		UInt32_t				flagShader;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !BASE_SHADER_H

