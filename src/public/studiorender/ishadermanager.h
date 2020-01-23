//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISHADERMANAGER_H
#define ISHADERMANAGER_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IShader;

	//---------------------------------------------------------------------//

	class IShaderManager
	{
	public:
		virtual ~IShaderManager() {}
		virtual bool			LoadShaderDLL( const char* FullPath ) = 0;
		virtual void			UnloadShaderDLL( const char* FullPath ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISHADERMANAGER_H

