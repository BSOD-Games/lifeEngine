//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "common/meshdescriptor.h"
#include "studiorender/studiovertexelement.h"

#include "quad.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Quad::Quad()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Quad::~Quad()
{}

// ------------------------------------------------------------------------------------ //
// Создать квадрат
// ------------------------------------------------------------------------------------ //
void le::Quad::Create( const Vector2D_t& Size )
{
	if ( !mesh.IsCreated() ) mesh.Delete();

	UInt32_t		quadIndeces[] = { 0, 1, 2, 1, 3, 2 };
	Vector3D_t		quadVerteces[] =
	{
		{ -Size.x, -Size.y, 0 },
		{ Size.x, -Size.y, 0 },
		{ -Size.x, Size.y, 0 },
		{ Size.x, Size.y, 0 }
	};

	StudioVertexElement			quadVertexElement;
	quadVertexElement.count = 3;
	quadVertexElement.type = VET_FLOAT;

	MeshDescriptor	quadDescriptor = {};
	quadDescriptor.indeces = quadIndeces;
	quadDescriptor.verteces = quadVerteces;
	quadDescriptor.primitiveType = PT_TRIANGLES;
	quadDescriptor.sizeVerteces = 4 * sizeof( Vector3D_t );
	quadDescriptor.countIndeces = 6;
	quadDescriptor.vertexElements = &quadVertexElement;
	quadDescriptor.countVertexElements = 1;

	mesh.Create( quadDescriptor );
}

// ------------------------------------------------------------------------------------ //
// Получить количество индексов вершин
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Quad::GetCountIndeces() const
{
	return 6;
}

// ------------------------------------------------------------------------------------ //
// Получить стартовый индекс вершины
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Quad::GetStartIndex() const
{
	return 0;
}