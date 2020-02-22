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

#include "common/shaderparaminfo.h"
#include "stdshaders/ishader.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IGPUProgram;
	struct ShaderDescriptor;

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
		bool							LoadShader( const ShaderDescriptor& ShaderDescriptor, const std::vector< const char* >& Defines, UInt32_t Flags = 0 );
		IGPUProgram*					GetGPUProgram( UInt32_t Flags ) const;

		std::vector< ShaderParamInfo >						shaderParams;

	private:
		std::unordered_map< UInt32_t, IGPUProgram* >		gpuPrograms;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !BASE_SHADER_H

