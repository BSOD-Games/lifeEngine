//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef VERTEX_ARRAY_OBJECT
#define VERTEX_ARRAY_OBJECT

#include <GL/glew.h>

#include "common/types.h"
#include "vertexbufferobject.h"
#include "indexbufferobject.h"
#include "vertexbufferlayout.h"

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//

	class VertexArrayObject
	{
	public:
		VertexArrayObject();
		~VertexArrayObject();

		inline void					Create()
		{
			if ( handle != 0 ) return;
			glGenVertexArrays( 1, &handle );
		}

		inline void					Delete()
		{
			if ( handle == 0 ) return;
			glDeleteVertexArrays( 1, &handle );
		}

		void						AddBuffer( VertexBufferObject& VertexBufferObject, VertexBufferLayout& VertexBufferLayout );
		void						AddBuffer( IndexBufferObject& IndexBufferObject );

		inline void					Bind() const
		{
			if ( handle == 0 ) return;
			glBindVertexArray( handle );
		}

		static inline void			Unbind()
		{ 
			glBindVertexArray( 0 ); 
		}
			
		inline UInt32_t				GetHandle() const
		{
			return handle;
		}

		inline bool					IsCreate() const
		{
			return handle > 0;
		}

	private:
		UInt32_t			handle;	
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !VERTEX_ARRAY_OBJECT
