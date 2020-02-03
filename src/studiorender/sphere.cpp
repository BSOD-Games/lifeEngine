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

#include "sphere.h"

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Sphere::Sphere()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Sphere::~Sphere()
{}

// ------------------------------------------------------------------------------------ //
// Создать квадрат
// ------------------------------------------------------------------------------------ //
void le::Sphere::Create( float Radius )
{
	if ( !mesh.IsCreated() ) mesh.Delete();

	UInt32_t		sphereIndeces[] = 
    { 
        #define INCLUDE_SPHERE_INDECES 
        #include "mesh_primitives/meshsphere.h" 
        #undef INCLUDE_SPHERE_INDECES
    };

	Vector3D_t		sphereVerteces[] =
	{
        #define INCLUDE_SPHERE_VERTECES 
        #include "mesh_primitives/meshsphere.h" 
        #undef INCLUDE_SPHERE_VERTECES
	};

	StudioVertexElement			sphereVertexElement;
	sphereVertexElement.count = 3;
	sphereVertexElement.type = VET_FLOAT;

	MeshDescriptor	sphereDescriptor = {};
	sphereDescriptor.indeces = sphereIndeces;
	sphereDescriptor.verteces = sphereVerteces;
	sphereDescriptor.primitiveType = PT_TRIANGLES;
	sphereDescriptor.sizeVerteces = 258 * sizeof( Vector3D_t );
	sphereDescriptor.countIndeces = 1440;
	sphereDescriptor.vertexElements = &sphereVertexElement;
	sphereDescriptor.countVertexElements = 1;

	mesh.Create( sphereDescriptor );
}

// ------------------------------------------------------------------------------------ //
// Получить количество индексов вершин
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Sphere::GetCountIndeces() const
{
	return 1440;
}

// ------------------------------------------------------------------------------------ //
// Получить стартовый индекс вершины
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Sphere::GetStartIndex() const
{
	return 0;
}