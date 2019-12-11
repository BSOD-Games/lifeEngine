//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef INDEX_BUFFER_OBJECT_H
#define INDEX_BUFFER_OBJECT_H

#include <GL/glew.h>

#include "common/types.h"
#include "engine/lifeengine.h"
#include "typeusagebuffer.h"

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//

	class IndexBufferObject
	{
	public:
		IndexBufferObject( TYPE_USAGE_BUFFER TypeUsage = TUB_STATIC );
		~IndexBufferObject();

		inline void					Create()
		{
			if ( handle != 0 ) return;
			glGenBuffers( 1, &handle );
		}

		inline void					Delete()
		{
			if ( handle == 0 ) return;
			glDeleteBuffers( 1, &handle );
		}

		inline void					Allocate( const void* Data, UInt32_t Size )
		{
			if ( handle == 0 ) return;
			LIFEENGINE_ASSERT( Data );

			glBufferData( GL_ELEMENT_ARRAY_BUFFER, Size, Data, typeUsage );
		}

        inline void					Update( const void* Data, UInt32_t Size, UInt32_t Offset )
        {
			if ( handle == 0 ) return;
			LIFEENGINE_ASSERT( Data );

            glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, Offset, Size, Data );
        }

		inline void					Bind()
		{
			if ( handle == 0 ) return;
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, handle );
		}

		static inline void			Unbind()
		{
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		}

		inline void					SetTypeUsage( TYPE_USAGE_BUFFER TypeUsage )
		{
			typeUsage = TypeUsage;
		}
			
		inline UInt32_t				GetHandle()
		{
			return handle;
		}

		inline TYPE_USAGE_BUFFER	GetTypeUsage()
		{
			return typeUsage;
		}

		inline bool					IsCreate()
		{
			return handle > 0;
		}

	private:
		TYPE_USAGE_BUFFER		typeUsage;	
		UInt32_t				handle;
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !INDEX_BUFFER_OBJECT_H
