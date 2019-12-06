//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISHADER_H
#define ISHADER_H

#include "common/types.h"
#include "materialsystem/imaterialvar.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ShaderParamInfo;

	//---------------------------------------------------------------------//

	class IShader
	{
	public:
		virtual bool					InitInstance( UInt32_t CountParams, IMaterialVar** MaterialVars ) = 0;
		virtual void					OnDrawElements( UInt32_t CountParams, IMaterialVar** MaterialVars /*, ICamera* Camera */ ) = 0;

		virtual const char*				GetName() const = 0;
		virtual const char*				GetFallbackShader() const = 0;
		virtual const char*				GetParamName( UInt32_t Index ) const = 0;
		virtual const char*				GetParamHelp( UInt32_t Index ) const = 0;
		virtual const char*				GetParamDefault( UInt32_t Index ) const = 0;
		virtual MATERIAL_VAR_TYPE		GetParamType( UInt32_t Index ) const =  0;
		virtual UInt32_t				GetCountParams() const = 0;
		virtual ShaderParamInfo**		GetParams() const = 0;
		virtual ShaderParamInfo*		GetParam( UInt32_t Index ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISHADER_H

