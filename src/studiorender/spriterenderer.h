//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <vector>

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//
    
	class IMaterialInternal;
	class ICamera;
	class VertexArrayObject;

	//---------------------------------------------------------------------//

	class SpriteRenderer
	{
	public:

		//---------------------------------------------------------------------//

		struct RenderObject
		{
			VertexArrayObject*		vertexArrayObject;
			IMaterialInternal*		material;
			UInt32_t				startVertexIndex;
			UInt32_t				startIndex;
			UInt32_t				countIndeces;
			UInt32_t				primitiveType;
			Matrix4x4_t				transformation;
		};

		//---------------------------------------------------------------------//

		// SpriteRenderer
		SpriteRenderer();
		~SpriteRenderer();

		bool			Initialize();
		void			Render( const std::vector< RenderObject >& Objects, ICamera* Camera );

	private:
		bool			isInitialize;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SPRITERENDERER_H

