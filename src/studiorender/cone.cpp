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

#include "cone.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Cone::Cone()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Cone::~Cone()
{}

// ------------------------------------------------------------------------------------ //
// Создать квадрат
// ------------------------------------------------------------------------------------ //
void le::Cone::Create()
{
	if ( !mesh.IsCreated() ) mesh.Delete();

	UInt32_t		coneIndeces[] = 
    { 
        #define INCLUDE_CONE_INDECES 
        #include "mesh_primitives/meshcone.h" 
        #undef INCLUDE_CONE_INDECES
    };

	Vector3D_t		coneVerteces[] =
	{
        #define INCLUDE_CONE_VERTECES 
        #include "mesh_primitives/meshcone.h" 
        #undef INCLUDE_CONE_VERTECES
	};

	StudioVertexElement			coneVertexElement;
	coneVertexElement.count = 3;
	coneVertexElement.type = VET_FLOAT;

	MeshDescriptor	coneDescriptor = {};
	coneDescriptor.indeces = coneIndeces;
	coneDescriptor.verteces = coneVerteces;
	coneDescriptor.primitiveType = PT_TRIANGLES;
	coneDescriptor.sizeVerteces = 258 * sizeof( Vector3D_t );
	coneDescriptor.countIndeces = 1440;
	coneDescriptor.vertexElements = &coneVertexElement;
	coneDescriptor.countVertexElements = 1;

	mesh.Create( coneDescriptor );
}

// ------------------------------------------------------------------------------------ //
// Получить количество индексов вершин
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Cone::GetCountIndeces() const
{
	return 4920;
}

// ------------------------------------------------------------------------------------ //
// Получить стартовый индекс вершины
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Cone::GetStartIndex() const
{
	return 0;
}