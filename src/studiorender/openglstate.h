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

	class OpenGLState
	{
	public:
		friend class StudioRender;

		static void				EnableDepthTest( bool Enable = true );
		static void				EnableDepthWrite( bool Enable = true );
		static void				EnableBlend( bool Enable = true );
		static void				EnableCullFace( bool Enable =  true );

		static void				SetCullFaceType( CULLFACE_TYPE CullFaceType );

	private:
		static void				Initialize();

		static bool				isDepthTest;
		static bool				isDepthWrite;
		static bool				isBlend;
		static bool				isCullFace;
		static CULLFACE_TYPE	cullFaceType;
	};	

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !OPENGL_STATE_H

