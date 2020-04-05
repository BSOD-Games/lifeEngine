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

namespace scripts_api
{
	#include "model.h"
}

#define		MODEL( Object ) static_cast< le::Model* >( Object )

// ------------------------------------------------------------------------------------ //
// Move model
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_Move( model_t Model, vec3_t FactorMove )
{
	if ( !Model ) return;
	MODEL( Model )->Move( { FactorMove.x, FactorMove.y, FactorMove.z } );
}

// ------------------------------------------------------------------------------------ //
// Rotate model
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_EulerRotate( model_t Model, vec3_t FactorRotate )
{
	if ( !Model ) return;
	MODEL( Model )->Rotate( { glm::radians( FactorRotate.x ), glm::radians( FactorRotate.y ), glm::radians( FactorRotate.z ) } );
}

// ------------------------------------------------------------------------------------ //
// Rotate model
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_QuatRotate( model_t Model, quat_t FactorRotate )
{
	if ( !Model ) return;
	MODEL( Model )->Rotate( { FactorRotate.w, FactorRotate.x, FactorRotate.y, FactorRotate.z } );
}

// ------------------------------------------------------------------------------------ //
// Scale model
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_Scale( model_t Model, vec3_t FactorScale )
{
	if ( !Model ) return;
	MODEL( Model )->Scale( { FactorScale.x, FactorScale.y, FactorScale.z } );
}

// ------------------------------------------------------------------------------------ //
// Set position model
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_SetPosition( model_t Model, vec3_t Position )
{
	if ( !Model ) return;
	MODEL( Model )->SetPosition( { Position.x, Position.y, Position.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation model
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_SetEulerRotation( model_t Model, vec3_t Rotation )
{
	if ( !Model ) return;
	MODEL( Model )->SetRotation( { Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation model
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_SetQuatRotation( model_t Model, quat_t Rotation )
{
	if ( !Model ) return;
	MODEL( Model )->SetRotation( { Rotation.w, Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set scale model
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_SetScale( model_t Model, vec3_t Scale )
{
	if ( !Model ) return;
	MODEL( Model )->SetScale( { Scale.x, Scale.y, Scale.z } );
}

// ------------------------------------------------------------------------------------ //
// Get position model
// ------------------------------------------------------------------------------------ //
scripts_api::vec3_t scripts_api::Model_GetPosition( model_t Model )
{
	if ( !Model ) return { 0.f, 0.f, 0.f };
	le::Vector3D_t		position = MODEL( Model )->GetPosition();
	return { position.x, position.y, position.z };
}

// ------------------------------------------------------------------------------------ //
// Get rotation model
// ------------------------------------------------------------------------------------ //
scripts_api::quat_t scripts_api::Model_GetRotation( model_t Model )
{
	if ( !Model ) return { 0.f, 0.f, 0.f, 0.f };
	le::Quaternion_t		rotation = MODEL( Model )->GetRotation();
	return { rotation.w, rotation.x, rotation.y, rotation.z };
}

// ------------------------------------------------------------------------------------ //
// Get scale model
// ------------------------------------------------------------------------------------ //
scripts_api::vec3_t scripts_api::Model_GetScale( model_t Model )
{
	if ( !Model ) return { 0.f, 0.f, 0.f };
	le::Vector3D_t		scale = MODEL( Model )->GetScale();
	return { scale.x, scale.y, scale.z };
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_Delete( model_t Model )
{
	if ( !Model ) return;

	le::Model*			object = MODEL( Model );
	if ( object->GetCountReferences() <= 1 )
		object->Release();
	else
		object->DecrementReference();

	object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Set mesh
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_SetMesh( model_t Model, mesh_t Mesh )
{
	if ( !Model || !Mesh ) return;
	MODEL( Model )->SetMesh( ( le::IMesh* ) Mesh );
}

// ------------------------------------------------------------------------------------ //
// Set start face
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_SetStartFace( model_t Model, uint32_t StartFace )
{
	if ( !Model ) return;
	MODEL( Model )->SetStartFace( StartFace );
}

// ------------------------------------------------------------------------------------ //
// Set count face
// ------------------------------------------------------------------------------------ //
void scripts_api::Model_SetCountFace( model_t Model, uint32_t CountFace )
{
	if ( !Model ) return;
	MODEL( Model )->SetCountFace( CountFace );
}

// ------------------------------------------------------------------------------------ //
// Get mesh
// ------------------------------------------------------------------------------------ //
scripts_api::mesh_t scripts_api::Model_GetMesh( model_t Model )
{
	if ( !Model ) return nullptr;
	return MODEL( Model )->GetMesh();
}

// ------------------------------------------------------------------------------------ //
// Get start face
// ------------------------------------------------------------------------------------ //
uint32_t scripts_api::Model_GetStartFace( model_t Model )
{
	if ( !Model ) return 0;
	return MODEL( Model )->GetStartFace();
}

// ------------------------------------------------------------------------------------ //
// Get count face
// ------------------------------------------------------------------------------------ //
uint32_t scripts_api::Model_GetCountFace( model_t Model )
{
	if ( !Model ) return 0;
	return MODEL( Model )->GetCountFace();
}
