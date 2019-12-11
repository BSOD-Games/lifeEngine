//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include <GL/glew.h>
#include <vector>

#include "common/types.h"

//----------------------------------------------------------------------//

namespace le
{
	//----------------------------------------------------------------------//

	struct VertexBufferElement
	{
		static UInt32_t			GetSizeOfType( UInt32_t Type );

		UInt32_t			type;	
		UInt32_t			count;	
		UInt8_t				normalized;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();

		inline void				PushFloat( UInt32_t Count )
		{
			elements.push_back( { GL_FLOAT, Count, GL_FALSE } );
			stride += Count * VertexBufferElement::GetSizeOfType( GL_FLOAT );
		}	

		inline void				PushUInt( UInt32_t Count )
		{
			elements.push_back( { GL_UNSIGNED_INT, Count, GL_FALSE } );
			stride += Count * VertexBufferElement::GetSizeOfType( GL_UNSIGNED_INT );
		}

		inline void				PushUByte( UInt32_t Count )
		{
			elements.push_back( { GL_UNSIGNED_BYTE, Count, GL_TRUE } );
			stride += Count * VertexBufferElement::GetSizeOfType( GL_UNSIGNED_BYTE );
		}	

		inline void				Clear()
		{
			elements.clear();
		}

		const UInt32_t&									GetStride();
		const std::vector< VertexBufferElement >&		GetElements();

	private:
		UInt32_t								stride;	
		std::vector< VertexBufferElement >		elements;
	};

	//----------------------------------------------------------------------//
}

//----------------------------------------------------------------------//

#endif // !VERTEX_BUFFER_LAYOUT_H
