//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef TESTSHADER_H
#define TESTSHADER_H

#include <vector>

#include "common/shaderparaminfo.h"
#include "stdshaders/ishader.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class TestShader : public IShader
	{
	public:
		// IShader
		virtual bool					InitInstance( UInt32_t CountParams, IMaterialVar** MaterialVars );
		virtual void					OnDrawElements( UInt32_t CountParams, IMaterialVar** MaterialVars /*, ICamera* Camera */ );

		virtual const char*				GetName() const;
		virtual const char*				GetFallbackShader() const;
		virtual const char*				GetParamName( UInt32_t Index ) const;
		virtual const char*				GetParamHelp( UInt32_t Index ) const;
		virtual const char*				GetParamDefault( UInt32_t Index ) const;
		virtual MATERIAL_VAR_TYPE		GetParamType( UInt32_t Index ) const;
		virtual UInt32_t				GetCountParams() const;
		virtual ShaderParamInfo**		GetParams() const;
		virtual ShaderParamInfo*		GetParam( UInt32_t Index ) const;

		// TestShader
		TestShader();

	private:
		std::vector<ShaderParamInfo>			shaderParams;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADERDLL_H

