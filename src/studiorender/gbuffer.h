//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef GBUFFER_H
#define GBUFFER_H

#include "common/types.h"
#include "studiorender/texture.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class GBuffer
	{
	public:

		//---------------------------------------------------------------------//

		enum TEXTURE_TYPE
		{
			TT_POSITION,
			TT_ALBEDO,
			TT_NORMAL
		};

		//---------------------------------------------------------------------//

		enum BIND_TYPE
		{
			BT_NONE,
			BT_GEOMETRY,
			BT_LIGHT
		};

		//---------------------------------------------------------------------//

		GBuffer();
		~GBuffer();

		bool			Initialize( const Vector2DInt_t& WindowSize );
		void			Delete();
		void			Bind( BIND_TYPE BindType );
		void			Unbind();
		void			ShowBuffers();
		void			ShowFinalFrame();

		bool			IsInitialize() const;

	private:
		bool			isInitialize;
		Vector2DInt_t	windowSize;

		UInt32_t		handle_frameBuffer;
		Texture			depth;
		Texture			albedoSpecular;
		Texture			normalShininess;
		Texture			emission;
		Texture 		finalFrame;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !GBUFFER_H

