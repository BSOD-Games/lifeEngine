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
	mesh = Mesh;

	if ( mesh )
	{
		localMin = mesh->GetMin();
		localMax = mesh->GetMax();
	}
}

// ------------------------------------------------------------------------------------ //
// Задать минимальную точку в модели 
// ------------------------------------------------------------------------------------ //
void le::Model::SetMin( const Vector3D_t& MinPosition )
{
	localMin = MinPosition;
	min = ( localMin + position ) * rotation * scale;
}

// ------------------------------------------------------------------------------------ //
// Задать максимальную точку в модели
// ------------------------------------------------------------------------------------ //
void le::Model::SetMax( const Vector3D_t& MaxPosition )
{
	localMax = MaxPosition;
	max = ( localMax + position ) * rotation * scale;
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
const le::Vector3D_t& le::Model::GetMin() const
{
	return min;
}

// ------------------------------------------------------------------------------------ //
// Получить максимальную точку в модели
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::Model::GetMax() const
{
	return max;
}

// ------------------------------------------------------------------------------------ //
// Сместить
// ------------------------------------------------------------------------------------ //
void le::Model::Move( const Vector3D_t& FactorMove )
{
	position += FactorMove;
	min += FactorMove;
	max += FactorMove;
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

	min = rotation * min;
	max = rotation * max;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Повернуть кватернионом
// ------------------------------------------------------------------------------------ //
void le::Model::Rotate( const Quaternion_t& FactorRotate )
{
	rotation *= FactorRotate;
	min = rotation * min;
	max = rotation * max;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Отмасштабировать
// ------------------------------------------------------------------------------------ //
void le::Model::Scale( const Vector3D_t& FactorScale )
{
	scale += FactorScale;
	min *= FactorScale;
	max *= FactorScale;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Задать позицию
// ------------------------------------------------------------------------------------ //
void le::Model::SetPosition( const Vector3D_t& Position )
{
	position = Position;
	min = ( localMin + position ) * rotation * scale;
	max = ( localMax + position ) * rotation * scale;
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

	min = ( localMin + position ) * rotation * scale;
	max = ( localMax + position ) * rotation * scale;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Задать поворот в кватернионе
// ------------------------------------------------------------------------------------ //
void le::Model::SetRotation( const Quaternion_t& Rotation )
{
	rotation = Rotation;
	min = ( localMin + position ) * rotation * scale;
	max = ( localMax + position ) * rotation * scale;
	isNeedUpdateTransformation = true;
}

// ------------------------------------------------------------------------------------ //
// Задать масштаб
// ------------------------------------------------------------------------------------ //
void le::Model::SetScale( const Vector3D_t& Scale )
{
	scale = Scale;
	min = ( localMin + position ) * rotation * scale;
	max = ( localMax + position ) * rotation * scale;
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
	{
		transformation = glm::translate( position ) * glm::mat4_cast( rotation ) * glm::scale( scale );
		isNeedUpdateTransformation = false;
	}

	return transformation;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Model::Model() :
	isNeedUpdateTransformation( true ),
	mesh( nullptr ), 
	position( 0.f ),
	rotation( 1.f, 0.f, 0.f, 0.f ),
	scale( 1.f )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Model::~Model()
{}