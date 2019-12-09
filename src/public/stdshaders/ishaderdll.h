//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISHADERDLL_H
#define ISHADERDLL_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;
	class IShader;

	//---------------------------------------------------------------------//

	class IShaderDLL
	{
	public:
		virtual bool				Initialize( IEngine* Engine ) = 0;

		virtual UInt32_t			GetShaderCount() const = 0;
		virtual IShader*			GetShader( UInt32_t Index ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISHADERDLL_H

