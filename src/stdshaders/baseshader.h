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
		bool							LoadShader( const ShaderDescriptor& ShaderDescriptor );

		IGPUProgram*							gpuProgram;
		std::vector< ShaderParamInfo >			shaderParams;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !BASE_SHADER_H

