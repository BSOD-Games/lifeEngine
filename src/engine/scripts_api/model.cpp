//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/model.h"
#include "model.h"

// ------------------------------------------------------------------------------------ //
// Move model
// ------------------------------------------------------------------------------------ //
void Model_Move( void* Model, vec3_t FactorMove )
{
	if ( !Model ) return;
	static_cast< le::Model* >( Model )->Move( { FactorMove.x, FactorMove.y, FactorMove.z } );
}

// ------------------------------------------------------------------------------------ //
// Rotate model
// ------------------------------------------------------------------------------------ //
void Model_EulerRotate( void* Model, vec3_t FactorRotate )
{
	if ( !Model ) return;
	static_cast< le::Model* >( Model )->Rotate( { glm::radians( FactorRotate.x ), glm::radians( FactorRotate.y ), glm::radians( FactorRotate.z ) } );
}

// ------------------------------------------------------------------------------------ //
// Rotate model
// ------------------------------------------------------------------------------------ //
void Model_QuatRotate( void* Model, quat_t FactorRotate )
{
	if ( !Model ) return;
	static_cast< le::Model* >( Model )->Rotate( { FactorRotate.w, FactorRotate.x, FactorRotate.y, FactorRotate.z } );
}

// ------------------------------------------------------------------------------------ //
// Scale model
// ------------------------------------------------------------------------------------ //
void Model_Scale( void* Model, vec3_t FactorScale )
{
	if ( !Model ) return;
	static_cast< le::Model* >( Model )->Scale( { FactorScale.x, FactorScale.y, FactorScale.z } );
}

// ------------------------------------------------------------------------------------ //
// Set position model
// ------------------------------------------------------------------------------------ //
void Model_SetPosition( void* Model, vec3_t Position )
{
	if ( !Model ) return;
	static_cast< le::Model* >( Model )->SetPosition( { Position.x, Position.y, Position.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation model
// ------------------------------------------------------------------------------------ //
void Model_SetEulerRotation( void* Model, vec3_t Rotation )
{
	if ( !Model ) return;
	static_cast< le::Model* >( Model )->SetRotation( { Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation model
// ------------------------------------------------------------------------------------ //
void Model_SetQuatRotation( void* Model, quat_t Rotation )
{
	if ( !Model ) return;
	static_cast< le::Model* >( Model )->SetRotation( { Rotation.w, Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set scale model
// ------------------------------------------------------------------------------------ //
void Model_SetScale( void* Model, vec3_t Scale )
{
	if ( !Model ) return;
	static_cast< le::Model* >( Model )->SetScale( { Scale.x, Scale.y, Scale.z } );
}

// ------------------------------------------------------------------------------------ //
// Get position model
// ------------------------------------------------------------------------------------ //
vec3_t Model_GetPosition( void* Model )
{
	if ( !Model ) return { 0.f, 0.f, 0.f };
	le::Vector3D_t		position = static_cast< le::Model* >( Model )->GetPosition();
	return { position.x, position.y, position.z };
}

// ------------------------------------------------------------------------------------ //
// Get rotation model
// ------------------------------------------------------------------------------------ //
quat_t Model_GetRotation( void* Model )
{
	if ( !Model ) return { 0.f, 0.f, 0.f, 0.f };
	le::Quaternion_t		rotation = static_cast< le::Model* >( Model )->GetRotation();
	return { rotation.w, rotation.x, rotation.y, rotation.z };
}

// ------------------------------------------------------------------------------------ //
// Get scale model
// ------------------------------------------------------------------------------------ //
vec3_t Model_GetScale( void* Model )
{
	if ( !Model ) return { 0.f, 0.f, 0.f };
	le::Vector3D_t		scale = static_cast< le::Model* >( Model )->GetScale();
	return { scale.x, scale.y, scale.z };
}
