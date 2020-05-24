//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/sprite.h"
#include "engine/material.h"

namespace scripts_api
{
#include "sprite.h"
}

#define SPRITE( Object )		( static_cast< le::Sprite* >( Object ) )

// ------------------------------------------------------------------------------------ //
// Create sprite
// ------------------------------------------------------------------------------------ //
scripts_api::sprite_t scripts_api::Sprite_Create()
{
	return new le::Sprite();
}

// ------------------------------------------------------------------------------------ //
// Delete sprite
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_Delete( sprite_t Object )
{
	if ( !Object ) return;

	if ( SPRITE( Object )->GetCountReferences() <= 1 )
		SPRITE( Object )->Release();
	else
		SPRITE( Object )->DecrementReference();

	Object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Initialize sprite
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Sprite_Initialize( sprite_t Object, vec2f_t Size, material_t Material, spriteType_t SpriteType )
{
	if ( !Object ) return B_FALSE;
	return SPRITE( Object )->Initialize( { Size.x, Size.y }, ( le::IMaterial* ) Material, ( le::SPRITE_TYPE ) SpriteType ) ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Set material
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_SetMaterial( sprite_t Object, material_t Material )
{
	if ( !Object || !Material ) return;
	SPRITE( Object )->SetMaterial( ( le::IMaterial* ) Material );
}

// ------------------------------------------------------------------------------------ //
// Set size
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_SetSize( sprite_t Object, vec2f_t Size )
{
	if ( !Object ) return;
	SPRITE( Object )->SetSize( { Size.x, Size.y } );
}

// ------------------------------------------------------------------------------------ //
// Set type
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_SetType( sprite_t Object, spriteType_t SpriteType )
{
	if ( !Object ) return;
	SPRITE( Object )->SetType( ( le::SPRITE_TYPE ) SpriteType );
}

// ------------------------------------------------------------------------------------ //
// Is create
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::Sprite_IsCreate( sprite_t Object )
{
	if ( !Object ) return B_FALSE;
	return SPRITE( Object )->IsCreated() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get material
// ------------------------------------------------------------------------------------ //
scripts_api::material_t scripts_api::Sprite_GetMaterial( sprite_t Object )
{
	if ( !Object ) return nullptr;
	return SPRITE( Object )->GetMaterial();
}

// ------------------------------------------------------------------------------------ //
// Get min
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Sprite_GetMin( sprite_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			min = SPRITE( Object )->GetMin();
	return { min.x, min.y, min.z };
}

// ------------------------------------------------------------------------------------ //
// Get max
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Sprite_GetMax( sprite_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			max = SPRITE( Object )->GetMax();
	return { max.x, max.y, max.z };
}

// ------------------------------------------------------------------------------------ //
// Get size
// ------------------------------------------------------------------------------------ //
scripts_api::vec2f_t scripts_api::Sprite_GetSize( sprite_t Object )
{
	if ( !Object ) return { 0.f, 0.f };

	const le::Vector2D_t&			size = SPRITE( Object )->GetSize();
	return { size.x, size.y };
}

// ------------------------------------------------------------------------------------ //
// Get type
// ------------------------------------------------------------------------------------ //
scripts_api::spriteType_t scripts_api::Sprite_GetType( sprite_t Object )
{
	if ( !Object ) return ST_SPRITE_STATIC;
	return ( scripts_api::spriteType_t ) SPRITE( Object )->GetType();
}

// ------------------------------------------------------------------------------------ //
// Move
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_Move( sprite_t Object, vec3f_t FactorMove )
{
	if ( !Object ) return;
	SPRITE( Object )->Move( { FactorMove.x, FactorMove.y, FactorMove.z } );
}

// ------------------------------------------------------------------------------------ //
// Rotate
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_EulerRotate( sprite_t Object, vec3f_t FactorRotate )
{
	if ( !Object ) return;
	SPRITE( Object )->Rotate( { FactorRotate.x, FactorRotate.y, FactorRotate.z } );
}

// ------------------------------------------------------------------------------------ //
// Rotate
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_QuatRotate( sprite_t Object, quat_t FactorRotate )
{
	if ( !Object ) return;
	SPRITE( Object )->Rotate( { FactorRotate.w, FactorRotate.x, FactorRotate.y, FactorRotate.z } );
}

// ------------------------------------------------------------------------------------ //
// Scale
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_Scale( sprite_t Object, vec3f_t FactorScale )
{
	if ( !Object ) return;
	SPRITE( Object )->Scale( { FactorScale.x, FactorScale.y, FactorScale.z } );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_SetPosition( sprite_t Object, vec3f_t Position )
{
	if ( !Object ) return;
	SPRITE( Object )->SetPosition( { Position.x, Position.y, Position.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotation
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_SetEulerRotation( sprite_t Object, vec3f_t Rotation )
{
	if ( !Object ) return;
	SPRITE( Object )->SetRotation( { Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set rotatin
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_SetQuatRotation( sprite_t Object, quat_t Rotation )
{
	if ( !Object ) return;
	SPRITE( Object )->SetRotation( { Rotation.w, Rotation.x, Rotation.y, Rotation.z } );
}

// ------------------------------------------------------------------------------------ //
// Set scale
// ------------------------------------------------------------------------------------ //
void scripts_api::Sprite_SetScale( sprite_t Object, vec3f_t Scale )
{
	if ( !Object ) return;
	SPRITE( Object )->SetScale( { Scale.x, Scale.y, Scale.z } );
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Sprite_GetPosition( sprite_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&		vec3 = SPRITE( Object )->GetPosition();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get rotation
// ------------------------------------------------------------------------------------ //
scripts_api::quat_t scripts_api::Sprite_GetRotation( sprite_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f, 0.f };

	const le::Quaternion_t&		quat = SPRITE( Object )->GetRotation();
	return { quat.x, quat.y, quat.z, quat.w };
}

// ------------------------------------------------------------------------------------ //
// Get scale
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::Sprite_GetScale( sprite_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&		vec3 = SPRITE( Object )->GetScale();
	return { vec3.x, vec3.y, vec3.z };
}
