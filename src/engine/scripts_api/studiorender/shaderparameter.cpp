//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/global.h"
#include "engine/ifactory.h"
#include "mathlib/gtc/type_ptr.hpp"
#include "engine/shaderparameter.h"

namespace scripts_api
{
#include "shaderparameter.h"
}

// ------------------------------------------------------------------------------------ //
// Clear shader parameter
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_Clear( shaderParameter_t Object )
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->Clear();
}

// ------------------------------------------------------------------------------------ //
// Set name shader parameter
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetName( shaderParameter_t Object, const char* Name )
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetName( Name );
}

// ------------------------------------------------------------------------------------ //
// Set value int
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetValueInt( shaderParameter_t Object, int Value)
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetValueInt( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value float
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetValueFloat( shaderParameter_t Object, float Value )
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetValueFloat( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value shader flag
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetValueShaderFlag(shaderParameter_t Object, bool_t Value)
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetValueShaderFlag( Value == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Set value vec2
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetValueVec2( shaderParameter_t Object, vec2f_t Value )
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetValueVector2D( { Value.x, Value.y } );
}

// ------------------------------------------------------------------------------------ //
// Set value vec3
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetValueVec3( shaderParameter_t Object, vec3f_t Value )
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetValueVector3D( { Value.x, Value.y, Value.z } );
}

// ------------------------------------------------------------------------------------ //
// Set value vec4
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetValueVec4( shaderParameter_t Object, vec4f_t Value )
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetValueVector4D( { Value.x, Value.y, Value.z, Value.w } );
}

// ------------------------------------------------------------------------------------ //
// Set value mat4
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetValueMat4( shaderParameter_t Object, mat4_t Value )
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetValueMatrix( glm::make_mat4( Value.matrix ) );
}

// ------------------------------------------------------------------------------------ //
// Set value texture
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_SetValueTexture(shaderParameter_t Object, texture_t Value)
{
	if ( !Object ) return;
	static_cast< le::ShaderParameter* >( Object )->SetValueTexture( ( le::ITexture* ) Value );
}

// ------------------------------------------------------------------------------------ //
// Is defined
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::ShaderParameter_IsDefined( shaderParameter_t Object )
{
	if ( !Object ) return B_FALSE;
	static_cast< le::ShaderParameter* >( Object )->IsDefined();
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* scripts_api::ShaderParameter_GetName( shaderParameter_t Object )
{
	if ( !Object ) return nullptr;
	static_cast< le::ShaderParameter* >( Object )->GetName();
}

// ------------------------------------------------------------------------------------ //
// Get type
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameterType_t scripts_api::ShaderParameter_GetType( shaderParameter_t Object )
{
	if ( !Object ) return SPT_INT;
	return ( shaderParameterType_t ) static_cast< le::ShaderParameter* >( Object )->GetType();
}

// ------------------------------------------------------------------------------------ //
// Get value int
// ------------------------------------------------------------------------------------ //
int scripts_api::ShaderParameter_GetValueInt( shaderParameter_t Object )
{
	if ( !Object ) return 0;
	static_cast< le::ShaderParameter* >( Object )->GetValueInt();
}

// ------------------------------------------------------------------------------------ //
// Get value float
// ------------------------------------------------------------------------------------ //
float scripts_api::ShaderParameter_GetValueFloat( shaderParameter_t Object )
{
	if ( !Object ) return 0.f;
	static_cast< le::ShaderParameter* >( Object )->GetValueFloat();
}

// ------------------------------------------------------------------------------------ //
// Get value shader flag
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::ShaderParameter_GetValueShaderFlag( shaderParameter_t Object )
{
	if ( !Object ) return B_FALSE;
	static_cast< le::ShaderParameter* >( Object )->GetValueShaderFlag() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get value vec2
// ------------------------------------------------------------------------------------ //
scripts_api::vec2f_t scripts_api::ShaderParameter_GetValueVec2( shaderParameter_t Object )
{
	if ( !Object ) return { 0.f, 0.f };

	const le::Vector2D_t&			vec2 = static_cast< le::ShaderParameter* >( Object )->GetValueVector2D();
	return  { vec2.x, vec2.y };
}

// ------------------------------------------------------------------------------------ //
// Get value vec3
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::ShaderParameter_GetValueVec3( shaderParameter_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec3 = static_cast< le::ShaderParameter* >( Object )->GetValueVector3D();
	return  { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get value vec4
// ------------------------------------------------------------------------------------ //
scripts_api::vec4f_t scripts_api::ShaderParameter_GetValueVec4( shaderParameter_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f, 0.f };

	const le::Vector4D_t&			vec4 = static_cast< le::ShaderParameter* >( Object )->GetValueVector4D();
	return  { vec4.x, vec4.y, vec4.z, vec4.w };
}

// ------------------------------------------------------------------------------------ //
// Get value mat4
// ------------------------------------------------------------------------------------ //
scripts_api::mat4_t scripts_api::ShaderParameter_GetValueMat4( shaderParameter_t Object )
{
	if ( !Object ) return { 0.f };

	mat4_t				mat4;
	const float*		array = glm::value_ptr( static_cast< le::ShaderParameter* >( Object )->GetValueMatrix() );
	memcpy( mat4.matrix, array, sizeof( le::Matrix4x4_t ) );

	return mat4;
}

// ------------------------------------------------------------------------------------ //
// Get value texture
// ------------------------------------------------------------------------------------ //
scripts_api::texture_t scripts_api::ShaderParameter_GetValueTexture( shaderParameter_t Object )
{
	if ( !Object ) return nullptr;
	return static_cast< le::ShaderParameter* >( Object )->GetValueTexture();
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void scripts_api::ShaderParameter_Delete( shaderParameter_t Object)
{
	if ( !Object ) return;

	le::ShaderParameter*			object = static_cast< le::ShaderParameter* >( Object );
	if ( object->GetCountReferences() <= 1 )
		object->Release();
	else
		object->DecrementReference();

	object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t scripts_api::ShaderParameter_Create()
{
	return new le::ShaderParameter();
}
