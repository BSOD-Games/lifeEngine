//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://gitlab.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "vertexarrayobject.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::VertexArrayObject::VertexArrayObject() :
	handle( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::VertexArrayObject::~VertexArrayObject()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Добавить буфер
// ------------------------------------------------------------------------------------ //
void le::VertexArrayObject::AddBuffer( VertexBufferObject& VertexBufferObject, VertexBufferLayout& VertexBufferLayout )
{
	if ( handle == 0 ) return;

	Bind();
	VertexBufferObject.Bind();

	const std::vector<VertexBufferElement>		elements = VertexBufferLayout.GetElements();
	uint32_t									offset = 0;

	for ( uint32_t index = 0, count = elements.size(); index < count; ++index )
	{
		VertexBufferElement			element = elements[ index ];
		
		glEnableVertexAttribArray( index );
		glVertexAttribPointer( index, element.count, element.type, element.normalized, VertexBufferLayout.GetStride(), (void*) offset );
		offset += element.count * VertexBufferElement::GetSizeOfType( element.type );
	}

	Unbind();
	VertexBufferObject.Unbind();
}

// ------------------------------------------------------------------------------------ //
// Добавить буфер
// ------------------------------------------------------------------------------------ //
void le::VertexArrayObject::AddBuffer( IndexBufferObject& IndexBufferObject )
{
	if ( handle == 0 ) return;

	Bind();
	IndexBufferObject.Bind();
	Unbind();
	IndexBufferObject.Unbind();
}