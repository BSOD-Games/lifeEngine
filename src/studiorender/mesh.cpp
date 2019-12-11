//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "engine/lifeengine.h"
#include "studiorender/studiovertexelement.h"

#include "mesh.h"

// ------------------------------------------------------------------------------------ //
// Создать меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Create( const MeshDescriptor& MeshDescriptor )
{
	if ( isCreated )		Delete();

	// Запоминаем материалы
	for ( UInt32_t index = 0; index < MeshDescriptor.countMaterials; ++index )
		materials.push_back( MeshDescriptor.materials[ index ] );

	// Запоминаем поверхности
	for ( UInt32_t index = 0; index < MeshDescriptor.countSurfaces; ++index )
		surfaces.push_back( MeshDescriptor.surfaces[ index ] );

	// Загружаем информацию о меше в GPU
	vertexArrayObject.Create();
	vertexBufferObject.Create();
	indexBufferObject.Create();

	vertexBufferObject.Bind();
	vertexBufferObject.Allocate( MeshDescriptor.verteces, MeshDescriptor.sizeVerteces );

	indexBufferObject.Bind();
	indexBufferObject.Allocate( MeshDescriptor.indeces, MeshDescriptor.countIndeces * sizeof( UInt32_t ) );

	VertexBufferLayout				vertexBufferLayout;
	for ( UInt32_t index = 0; index < MeshDescriptor.vertexFormat.countElements; ++index )
		switch ( MeshDescriptor.vertexFormat.elements[ index ].type )
		{
		case VET_FLOAT:
			vertexBufferLayout.PushFloat( MeshDescriptor.vertexFormat.elements[ index ].count );
			break;

		case VET_UNSIGNED_INT:
			vertexBufferLayout.PushUInt( MeshDescriptor.vertexFormat.elements[ index ].count );
			break;

		case VET_UNSIGNED_BYTE:
			vertexBufferLayout.PushUByte( MeshDescriptor.vertexFormat.elements[ index ].count );
			break;
		}

	vertexArrayObject.Bind();
	vertexArrayObject.AddBuffer( vertexBufferObject, vertexBufferLayout );
	vertexArrayObject.AddBuffer( indexBufferObject );

	vertexArrayObject.Unbind();
	vertexBufferObject.Unbind();
	indexBufferObject.Unbind();

	primitiveType = MeshDescriptor.primitiveType;
	isCreated = true;
}

// ------------------------------------------------------------------------------------ //
// Удалить меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Delete()
{
	vertexArrayObject.Delete();
	vertexBufferObject.Delete();
	indexBufferObject.Delete();

	// TODO: Реализовать удаление материалов

	surfaces.clear();
	isCreated = false;
}

// ------------------------------------------------------------------------------------ //
// Создан ли меш
// ------------------------------------------------------------------------------------ //
bool le::Mesh::IsCreated() const
{
	return isCreated;
}

// ------------------------------------------------------------------------------------ //
// Получить количество поверхностей
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Mesh::GetCountSurfaces() const
{
	return surfaces.size();
}

// ------------------------------------------------------------------------------------ //
// Получить поверхность
// ------------------------------------------------------------------------------------ //
const le::MeshSurface& le::Mesh::GetSurface( UInt32_t Index ) const
{
	if ( Index >= surfaces.size() ) return le::MeshSurface();
	return surfaces[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Получить массив поверхностей
// ------------------------------------------------------------------------------------ //
le::MeshSurface* le::Mesh::GetSurfaces() const
{
	return ( MeshSurface* ) surfaces.data();
}

// ------------------------------------------------------------------------------------ //
// Получить количество материалов
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Mesh::GetCountMaterials() const
{
	return materials.size();
}

// ------------------------------------------------------------------------------------ //
// Получить материал
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::Mesh::GetMaterial( UInt32_t Index ) const
{
	if ( Index >= materials.size() ) return nullptr;
	return materials[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Получить массив материалов
// ------------------------------------------------------------------------------------ //
le::IMaterial* le::Mesh::GetMaterials() const
{
	return ( IMaterial* ) materials.data();
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Mesh::Mesh() :
	isCreated( false ),
	primitiveType( PT_TRIANGLES )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Mesh::~Mesh()
{
	Delete();
}
