//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "engine/lifeengine.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//
	
	class IShader;

	//---------------------------------------------------------------------//

	class ShaderManager
	{
	public:
		// ShaderSystem
		ShaderManager();
		~ShaderManager();

		IShader*		FindShader( const char* Name ) const;

	private:
		std::vector< IShader* >			shaders;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADERMANAGER_H

