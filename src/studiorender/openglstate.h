//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef OPENGL_STATE_H
#define OPENGL_STATE_H

#include <cstddef>

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum CULLFACE_TYPE;

	//---------------------------------------------------------------------//

	struct OpenGLState
	{
		bool			isDepthTest;
		bool			isCullFace;
		bool			isBlend;

		CULLFACE_TYPE	cullFaceType;
	};

	//---------------------------------------------------------------------//

	struct Hash_OpenGLState
	{
		std::size_t			operator()( const OpenGLState& Right ) const;
	};

	//---------------------------------------------------------------------//

	bool					operator== ( const OpenGLState& Left, const OpenGLState& Right );

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !OPENGL_STATE_H

