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
		virtual UInt32_t				GetCountParams() const;
		virtual ShaderParamInfo*		GetParam( UInt32_t Index ) const;
		virtual ShaderParamInfo*		GetParams() const;

		// BaseShader
		BaseShader();
		virtual ~BaseShader();

	protected:
		bool								LoadShader( const std::string& Name, const std::string& Path, const std::vector< const char* >& Defines, UInt32_t Flags = 0 );

		IGPUProgram*						gpuProgram;
		std::vector< ShaderParamInfo >		shaderParams;

	private:
		std::string				nameShader;
		UInt32_t				flagShader;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !BASE_SHADER_H

