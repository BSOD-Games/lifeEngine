//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STATICMODELRENDERER_H
#define STATICMODELRENDERER_H

#include <vector>

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Texture;
	class IMaterialInternal;
	class ICamera;
	class VertexArrayObject;

	//---------------------------------------------------------------------//

	class StaticModelRenderer
	{
	public:

		//---------------------------------------------------------------------//

		struct RenderObject
		{
			VertexArrayObject*		vertexArrayObject;
			IMaterialInternal*		material;
			Texture*				lightmap;
			UInt32_t				startVertexIndex;
			UInt32_t				startIndex;
			UInt32_t				countIndeces;
			UInt32_t				primitiveType;
			Matrix4x4_t				transformation;
		};

		//---------------------------------------------------------------------//

		// StaticModelRenderer
		StaticModelRenderer();
		~StaticModelRenderer();

		bool			Initialize();
		void			Render( const std::vector< RenderObject >& Objects, ICamera* Camera );

	private:
		bool			isInitialize;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STATICMODELRENDERER_H

