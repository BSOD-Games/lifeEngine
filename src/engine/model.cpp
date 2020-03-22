//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "studiorender/imesh.h"
#include "model.h"

// ------------------------------------------------------------------------------------ //
// Задать меш
// ------------------------------------------------------------------------------------ //
void le::Model::SetMesh( IMesh* Mesh )
{
    if ( mesh )
    {
        if ( mesh->GetCountReferences() <= 1 )
            mesh->Release();
        else
            mesh->DecrementReference();
    }

	mesh = Mesh;

	if ( mesh )
	{        
		localMin = mesh->GetMin();
		localMax = mesh->GetMax();
		countFace = mesh->GetCountSurfaces();
		startFace = 0;

        mesh->IncrementReference();
		isNeedUpdateBoundingBox = true;
	}
}

// ------------------------------------------------------------------------------------ //
// Задать минимальную точку в модели 
// ------------------------------------------------------------------------------------ //
void le::Model::SetMin( const Vector3D_t& MinPosition )
{
	localMin = MinPosition;
	isNeedUpdateBoundingBox = true;
}

// ------------------------------------------------------------------------------------ //
// Задать максимальную точку в модели
// ------------------------------------------------------------------------------------ //
void le::Model::SetMax( const Vector3D_t& MaxPosition )
{
	localMax = MaxPosition;
	isNeedUpdateBoundingBox = true;
}

// ------------------------------------------------------------------------------------ //
// Задать начальный индекс поверхности модели
// ------------------------------------------------------------------------------------ //
void le::Model::SetStartFace( UInt32_t StartFace )
{
	startFace = StartFace;
}

// ------------------------------------------------------------------------------------ //
// Задать количество поверхностей модели
// ------------------------------------------------------------------------------------ //
void le::Model::SetCountFace( UInt32_t CountFace )
{
	countFace = CountFace;
}

// ------------------------------------------------------------------------------------ //
// Получить меш
// ------------------------------------------------------------------------------------ //
le::IMesh* le::Model::GetMesh() const
{
	return mesh;
}

// ------------------------------------------------------------------------------------ //
// Получить минимальную точку в модели 
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Model::GetMin()
{
	if ( isNeedUpdateBoundingBox )
		UpdateBoundingBox();

	return min;
}

// ------------------------------------------------------------------------------------ //
// Получить максимальную точку в модели
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Model::GetMax()
{
	if ( isNeedUpdateBoundingBox )
		UpdateBoundingBox();

	return max;
}

// ------------------------------------------------------------------------------------ //
// Получить начальный индекс поверхности модели
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Model::GetStartFace() const
{
	return startFace;
}

// ------------------------------------------------------------------------------------ //
// Получить количество поверхностей модели
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Model::GetCountFace() const
{
	return countFace;
}

// ------------------------------------------------------------------------------------ //
// Сместить
// ------------------------------------------------------------------------------------ //
void le::Model::Move( const Vector3D_t& FactorMove )
{
	position += FactorMove;

	isNeedUpdateBoundingBox = true;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Повернуть углами Эйлера
// ------------------------------------------------------------------------------------ //
void le::Model::Rotate( const Vector3D_t& FactorRotate )
{
	Vector3D_t			axis( sin( FactorRotate.x / 2.f ), sin( FactorRotate.y / 2.f ), sin( FactorRotate.z / 2.f ) );
	Vector3D_t			rotations( cos( FactorRotate.x / 2.f ), cos( FactorRotate.y / 2.f ), cos( FactorRotate.z / 2.f ) );

	rotation *= Quaternion_t( rotations.x, axis.x, 0, 0 ) * 
		Quaternion_t( rotations.y, 0, axis.y, 0 ) * 
		Quaternion_t( rotations.z, 0, 0, axis.z );

	isNeedUpdateBoundingBox = true;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Повернуть кватернионом
// ------------------------------------------------------------------------------------ //
void le::Model::Rotate( const Quaternion_t& FactorRotate )
{
	rotation *= FactorRotate;

	isNeedUpdateBoundingBox = true;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Отмасштабировать
// ------------------------------------------------------------------------------------ //
void le::Model::Scale( const Vector3D_t& FactorScale )
{
	scale += FactorScale;

	isNeedUpdateBoundingBox = true;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Задать позицию
// ------------------------------------------------------------------------------------ //
void le::Model::SetPosition( const Vector3D_t& Position )
{
	position = Position;

	isNeedUpdateBoundingBox = true;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Задать поворот в углах Эйлера
// ------------------------------------------------------------------------------------ //
void le::Model::SetRotation( const Vector3D_t& Rotation )
{
	Vector3D_t			axis( sin( Rotation.x / 2.f ), sin( Rotation.y / 2.f ), sin( Rotation.z / 2.f ) );
	Vector3D_t			rotations( cos( Rotation.x / 2.f ), cos( Rotation.y / 2.f ), cos( Rotation.z / 2.f ) );

	rotation = Quaternion_t( rotations.x, axis.x, 0, 0 ) *
		Quaternion_t( rotations.y, 0, axis.y, 0 ) *
		Quaternion_t( rotations.z, 0, 0, axis.z );

	isNeedUpdateBoundingBox = true;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Задать поворот в кватернионе
// ------------------------------------------------------------------------------------ //
void le::Model::SetRotation( const Quaternion_t& Rotation )
{
	rotation = Rotation;

	isNeedUpdateBoundingBox = true;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Задать масштаб
// ------------------------------------------------------------------------------------ //
void le::Model::SetScale( const Vector3D_t& Scale )
{
	scale = Scale;

	isNeedUpdateBoundingBox = true;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Получить позицию
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Model::GetPosition() const
{
	return position;
}

// ------------------------------------------------------------------------------------ //
// Получить поворот
// ------------------------------------------------------------------------------------ //
const le::Quaternion_t& le::Model::GetRotation() const
{
	return rotation;
}

// ------------------------------------------------------------------------------------ //
// Получить масштаб
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Model::GetScale() const
{
	return scale;
}

// ------------------------------------------------------------------------------------ //
// Получить матрицу трансформации
// ------------------------------------------------------------------------------------ //
const le::Matrix4x4_t& le::Model::GetTransformation()
{
	if ( isNeedUpdateTransformation )	
		UpdateTransformation();

	return transformation;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Model::Model() :
	isNeedUpdateTransformation( true ),
	isNeedUpdateBoundingBox( true ),
	mesh( nullptr ), 
	position( 0.f ),
	rotation( 1.f, 0.f, 0.f, 0.f ),
	scale( 1.f ),
	startFace( 0 ),
    countFace( 0 ),
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Model::~Model()
{
    if ( mesh )
    {
        if ( mesh->GetCountReferences() <= 1 )
            mesh->Release();
        else
            mesh->DecrementReference();
    }
}

// ------------------------------------------------------------------------------------ //
// Обновить матрицу трансформации
// ------------------------------------------------------------------------------------ //
void le::Model::UpdateTransformation()
{
	if ( !isNeedUpdateTransformation ) return;

	transformation = glm::translate( position ) * glm::mat4_cast( rotation ) * glm::scale( scale );
	isNeedUpdateTransformation = false;
}

// ------------------------------------------------------------------------------------ //
// Обновить BBox
// ------------------------------------------------------------------------------------ //
void le::Model::UpdateBoundingBox()
{
	if ( !isNeedUpdateBoundingBox ) return;

	Vector3D_t		min = ( rotation * localMin * scale ) + position;
	Vector3D_t		max = ( rotation * localMax * scale ) + position;

	this->min = Vector3D_t( glm::min( min.x, max.x ), glm::min( min.y, max.y ), glm::min( min.z, max.z ) );
	this->max = Vector3D_t( glm::max( max.x, min.x ), glm::max( max.y, min.y ), glm::max( max.z, min.z ) );

	isNeedUpdateBoundingBox = false;
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Model::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Model::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Model::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Model::GetCountReferences() const
{
    return countReferences;
}
