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
#include "engine/imaterial.h"
#include "engine/iconsolesystem.h"
#include "studiorender/itexture.h"
#include "studiorender/studiovertexelement.h"

#include "global.h"
#include "mesh.h"

// ------------------------------------------------------------------------------------ //
// Создать меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Create( const MeshDescriptor& MeshDescriptor )
{
	if ( ( MeshDescriptor.sizeVerteces > 0 && !MeshDescriptor.verteces ) ||
		 ( MeshDescriptor.countIndeces > 0 && !MeshDescriptor.indeces ) )
		return;

	if ( isCreated )		Delete();

	// Запоминаем материалы
	for ( UInt32_t index = 0; index < MeshDescriptor.countMaterials; ++index )
	{
		if ( !MeshDescriptor.materials[ index ] )		return;

		materials.push_back( MeshDescriptor.materials[ index ] );
		materials.back()->IncrementReference();
	}

	// Запоминаем карты освещений
	for ( UInt32_t index = 0; index < MeshDescriptor.countLightmaps; ++index )
	{
		if ( !MeshDescriptor.lightmaps[ index ] )		return;

		lightmaps.push_back( MeshDescriptor.lightmaps[ index ] );
		lightmaps.back()->IncrementReference();
	}

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
	for ( UInt32_t index = 0; index < MeshDescriptor.countVertexElements; ++index )
		switch ( MeshDescriptor.vertexElements[ index ].type )
		{
		case VET_FLOAT:
			vertexBufferLayout.PushFloat( MeshDescriptor.vertexElements[ index ].count );
			break;

		case VET_UNSIGNED_INT:
			vertexBufferLayout.PushUInt( MeshDescriptor.vertexElements[ index ].count );
			break;

		case VET_UNSIGNED_BYTE:
			vertexBufferLayout.PushUByte( MeshDescriptor.vertexElements[ index ].count );
			break;
		}

	vertexArrayObject.Bind();
	vertexArrayObject.AddBuffer( vertexBufferObject, vertexBufferLayout );
	vertexArrayObject.AddBuffer( indexBufferObject );

	vertexArrayObject.Unbind();
	vertexBufferObject.Unbind();
	indexBufferObject.Unbind();

	min = MeshDescriptor.min;
	max = MeshDescriptor.max;
	primitiveType = MeshDescriptor.primitiveType;
	isCreated = true;
}

// ------------------------------------------------------------------------------------ //
// Обновить меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Update( const void* Verteces, UInt32_t SizeVerteces, UInt32_t OffsetVerteces )
{
	if ( !isCreated ) return;

	UInt32_t        maxSize = OffsetVerteces + SizeVerteces;
	bool            isResizeVertexBuffer = OffsetVerteces == 0 && ( vertexBufferObject.GetSize() != maxSize );
	LIFEENGINE_ASSERT( !isResizeVertexBuffer && vertexBufferObject.GetSize() > maxSize );

	vertexBufferObject.Bind();

	if ( !isResizeVertexBuffer )
		vertexBufferObject.Update( Verteces, SizeVerteces, OffsetVerteces );
	else
		vertexBufferObject.Allocate( Verteces, SizeVerteces );

	vertexBufferObject.Unbind();
}

// ------------------------------------------------------------------------------------ //
// Обновить меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Update( UInt32_t* Indeces, UInt32_t CountIndeces, UInt32_t StartIdIndex )
{
	if ( !isCreated ) return;

	UInt32_t        maxCount = ( StartIdIndex + CountIndeces ) * sizeof( UInt32_t );
	bool            isResizeIndexBuffer = StartIdIndex == 0 && ( indexBufferObject.GetSize() != maxCount );
	LIFEENGINE_ASSERT( !isResizeIndexBuffer && indexBufferObject.GetSize() > maxCount );

	indexBufferObject.Bind();

	if ( !isResizeIndexBuffer )
		indexBufferObject.Update( Indeces, CountIndeces * sizeof( UInt32_t ), StartIdIndex * sizeof( UInt32_t ) );
	else
		indexBufferObject.Allocate( Indeces, CountIndeces * sizeof( UInt32_t ) );

	indexBufferObject.Unbind();
}

// ------------------------------------------------------------------------------------ //
// Обновить меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Update( MeshSurface* Surfaces, UInt32_t CountSurfaces, UInt32_t StartIdSurface )
{
	if ( !isCreated ) return;

	UInt32_t            maxCount = StartIdSurface + CountSurfaces;

	// If array is small - resize to max count surfaces and
	// fill empty surfaces
	if ( surfaces.size() < maxCount )
		surfaces.resize( maxCount, { 0, 0, 0, 0, 0 } );

	// Change surfaces
	for ( UInt32_t indexSurface = StartIdSurface, indexLocalSurface = 0; indexSurface < maxCount && indexLocalSurface < CountSurfaces; ++indexSurface, ++indexLocalSurface )
		surfaces[ indexSurface ] = Surfaces[ indexLocalSurface ];
}

// ------------------------------------------------------------------------------------ //
// Обновить меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Update( IMaterial** Materials, UInt32_t CountMaterials, UInt32_t StartIdMaterial )
{
	if ( !isCreated ) return;

	UInt32_t            maxCount = StartIdMaterial + CountMaterials;

	// If array is small - resize to max count materials and
	// fill nullptr's
	if ( materials.size() < maxCount )
		materials.resize( maxCount, nullptr );

	// Change materials
	for ( UInt32_t indexMaterial = StartIdMaterial, indexLocalMaterial = 0; indexMaterial < maxCount && indexLocalMaterial < CountMaterials; ++indexMaterial, ++indexLocalMaterial )
	{
		IMaterial* oldMaterial = materials[ indexMaterial ];
		IMaterial* newMaterial = Materials[ indexLocalMaterial ];

		if ( oldMaterial )
		{
			if ( oldMaterial->GetCountReferences() <= 0 )		oldMaterial->Release();
			else												oldMaterial->DecrementReference();
		}

		newMaterial->IncrementReference();
		materials[ indexMaterial ] = newMaterial;
	}
}

// ------------------------------------------------------------------------------------ //
// Обновить меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Update( ITexture** Lightmaps, UInt32_t CountLighmaps, UInt32_t StartIdLightmap )
{
	if ( !isCreated ) return;

	UInt32_t            maxCount = StartIdLightmap + CountLighmaps;

	// If array is small - resize to max count lightmaps and
	// fill nullptr's
	if ( lightmaps.size() < maxCount )
		lightmaps.resize( maxCount, nullptr );

	// Change materials
	for ( UInt32_t indexLightmap = StartIdLightmap, indexLocalLightmap = 0; indexLightmap < maxCount && indexLocalLightmap < CountLighmaps; ++indexLightmap, ++indexLocalLightmap )
	{
		ITexture* oldLightmap = lightmaps[ indexLightmap ];
		ITexture* newLightmap = Lightmaps[ indexLocalLightmap ];

		if ( oldLightmap )
		{
			if ( oldLightmap->GetCountReferences() <= 0 )		oldLightmap->Release();
			else												oldLightmap->DecrementReference();
		}

		newLightmap->IncrementReference();
		lightmaps[ indexLightmap ] = newLightmap;
	}
}

// ------------------------------------------------------------------------------------ //
// Удалить меш
// ------------------------------------------------------------------------------------ //
void le::Mesh::Delete()
{
	vertexArrayObject.Delete();
	vertexBufferObject.Delete();
	indexBufferObject.Delete();

	for ( auto it = materials.begin(), itEnd = materials.end(); it != itEnd; ++it )
	{
		if ( !( *it ) ) continue;

		if ( ( *it )->GetCountReferences() <= 1 )
			( *it )->Release();
		else
			( *it )->DecrementReference();
	}

	for ( auto it = lightmaps.begin(), itEnd = lightmaps.end(); it != itEnd; ++it )
	{
		if ( !( *it ) ) continue;

		if ( ( *it )->GetCountReferences() <= 1 )
			( *it )->Release();
		else
			( *it )->DecrementReference();
	}

	materials.clear();
	lightmaps.clear();
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
	if ( Index >= surfaces.size() ) return MeshSurface();
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
le::IMaterial** le::Mesh::GetMaterials() const
{
	return ( IMaterial** ) materials.data();
}

// ------------------------------------------------------------------------------------ //
// Получить количество карт освещения
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Mesh::GetCountLightmaps() const
{
	return lightmaps.size();
}

// ------------------------------------------------------------------------------------ //
// Получить карту освещения
// ------------------------------------------------------------------------------------ //
le::ITexture* le::Mesh::GetLightmap( UInt32_t Index ) const
{
	if ( Index >= lightmaps.size() ) return nullptr;
	return lightmaps[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Получить массив карт освещений
// ------------------------------------------------------------------------------------ //
le::ITexture** le::Mesh::GetLightmaps() const
{
	return ( ITexture** ) lightmaps.data();
}

// ------------------------------------------------------------------------------------ //
// Получить минимальную точку в меше
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Mesh::GetMin() const
{
	return min;
}

// ------------------------------------------------------------------------------------ //
// Получить максимульную точку в меше
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Mesh::GetMax() const
{
	return max;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Mesh::Mesh() :
	isCreated( false ),
	primitiveType( PT_TRIANGLES ),
	countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Mesh::~Mesh()
{
	Delete();
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Mesh::IncrementReference()
{
	++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Mesh::DecrementReference()
{
	--countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Mesh::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Mesh::GetCountReferences() const
{
	return countReferences;
}
