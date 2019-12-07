//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADERDLL_H
#define SHADERDLL_H

#include <vector>

#include "stdshaders/ishader.h"
#include "stdshaders/ishaderdll.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ShaderDLL : public IShaderDLL
	{
	public:
		// IShaderDLL
		virtual bool				Initialize( IEngine* Engine );
	
		virtual UInt32_t			GetShaderCount() const;
		virtual IShader*			GetShader( UInt32_t Index ) const;

		// ShaderDLL
		~ShaderDLL();

	private:
		std::vector< IShader* >				shaders;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADERDLL_H

