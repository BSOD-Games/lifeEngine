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
#include "engine/materialproxyvar.h"

namespace scripts_api
{
#include "materialproxyvar.h"
}

// ------------------------------------------------------------------------------------ //
// Clear
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_Clear( materialProxyVar_t Object )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->Clear();
}

// ------------------------------------------------------------------------------------ //
// Set name
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetName( materialProxyVar_t Object, const char* Name )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetName( Name );
}

// ------------------------------------------------------------------------------------ //
// Set value int
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueInt( materialProxyVar_t Object, int Value )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueInt( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value float
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueFloat( materialProxyVar_t Object, float Value )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueFloat( Value );
}

// ------------------------------------------------------------------------------------ //
// Set value bool
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueBool( materialProxyVar_t Object, bool_t Value )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueBool( Value == B_TRUE ? true : false );
}

// ------------------------------------------------------------------------------------ //
// Set value vec2
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueVec2(materialProxyVar_t Object, vec2f_t Value)
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueVector2D( { Value.x, Value.y } );
}

// ------------------------------------------------------------------------------------ //
// Set value vec3
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueVec3( materialProxyVar_t Object, vec3f_t Value )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueVector3D( { Value.x, Value.y, Value.z } );
}

// ------------------------------------------------------------------------------------ //
// Set value vec4
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueVec4( materialProxyVar_t Object, vec4f_t Value )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueVector4D( { Value.x, Value.y, Value.z, Value.w } );
}

// ------------------------------------------------------------------------------------ //
// Set value shader parameter
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueShaderParameter( materialProxyVar_t Object, shaderParameter_t Value )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueShaderParameter( ( le::IShaderParameter* ) Value );
}

// ------------------------------------------------------------------------------------ //
// Set value array float
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueArrayFloat(materialProxyVar_t Object, float* Array, uint32_t Count)
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueArrayFloat( Array, Count );
}

// ------------------------------------------------------------------------------------ //
// Set value array int
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueArrayInt( materialProxyVar_t Object, int* Array, uint32_t Count )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueArrayInt( Array, Count );
}

// ------------------------------------------------------------------------------------ //
// Set value array vec2
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueArrayVec2(materialProxyVar_t Object, vec2f_t* Array, uint32_t Count)
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueArrayVector2D( ( le::Vector2D_t* ) Array, Count );
}

// ------------------------------------------------------------------------------------ //
// Set value array vec3
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueArrayVec3( materialProxyVar_t Object, vec3f_t* Array, uint32_t Count )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueArrayVector3D( ( le::Vector3D_t* ) Array, Count );
}

// ------------------------------------------------------------------------------------ //
// Set value array vec4
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_SetValueArrayVec4( materialProxyVar_t Object, vec4f_t* Array, uint32_t Count )
{
	if ( !Object ) return;
	static_cast< le::MaterialProxyVar* >( Object )->SetValueArrayVector4D( ( le::Vector4D_t* ) Array, Count );
}

// ------------------------------------------------------------------------------------ //
// Is defined
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::MaterialProxyVar_IsDefined( materialProxyVar_t Object )
{
	if ( !Object ) return B_FALSE;
	return static_cast< le::MaterialProxyVar* >( Object )->IsDefined() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* scripts_api::MaterialProxyVar_GetName( materialProxyVar_t Object )
{
	if ( !Object ) return nullptr;
	return static_cast< le::MaterialProxyVar* >( Object )->GetName();
}

// ------------------------------------------------------------------------------------ //
// Get type
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxyVarType_t scripts_api::MaterialProxyVar_GetType( materialProxyVar_t Object )
{
	if ( !Object ) return MPVT_NONE;
	return ( materialProxyVarType_t ) static_cast< le::MaterialProxyVar* >( Object )->GetType();
}

// ------------------------------------------------------------------------------------ //
// Get value int
// ------------------------------------------------------------------------------------ //
int scripts_api::MaterialProxyVar_GetValueInt( materialProxyVar_t Object )
{
	if ( !Object ) return 0;
	return static_cast< le::MaterialProxyVar* >( Object )->GetValueInt();
}

// ------------------------------------------------------------------------------------ //
// Get value float
// ------------------------------------------------------------------------------------ //
float scripts_api::MaterialProxyVar_GetValueFloat( materialProxyVar_t Object )
{
	if ( !Object ) return 0.f;
	return static_cast< le::MaterialProxyVar* >( Object )->GetValueFloat();
}

// ------------------------------------------------------------------------------------ //
// Get value bool
// ------------------------------------------------------------------------------------ //
scripts_api::bool_t scripts_api::MaterialProxyVar_GetValueBool( materialProxyVar_t Object )
{
	if ( !Object ) return B_FALSE;
	return static_cast< le::MaterialProxyVar* >( Object )->GetValueBool() ? B_TRUE : B_FALSE;
}

// ------------------------------------------------------------------------------------ //
// Get value vec2
// ------------------------------------------------------------------------------------ //
scripts_api::vec2f_t scripts_api::MaterialProxyVar_GetValueVec2( materialProxyVar_t Object )
{
	if ( !Object ) return { 0.f, 0.f };

	const le::Vector2D_t&			vec2 = static_cast< le::MaterialProxyVar* >( Object )->GetValueVector2D();
	return { vec2.x, vec2.y };
}

// ------------------------------------------------------------------------------------ //
// Get value vec3
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t scripts_api::MaterialProxyVar_GetValueVec3( materialProxyVar_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f };

	const le::Vector3D_t&			vec3 = static_cast< le::MaterialProxyVar* >( Object )->GetValueVector3D();
	return { vec3.x, vec3.y, vec3.z };
}

// ------------------------------------------------------------------------------------ //
// Get value vec4
// ------------------------------------------------------------------------------------ //
scripts_api::vec4f_t scripts_api::MaterialProxyVar_GetValueVec4( materialProxyVar_t Object )
{
	if ( !Object ) return { 0.f, 0.f, 0.f, 0.f };

	const le::Vector4D_t&			vec4 = static_cast< le::MaterialProxyVar* >( Object )->GetValueVector4D();
	return { vec4.x, vec4.y, vec4.z, vec4.w };
}

// ------------------------------------------------------------------------------------ //
// Get value shader parameter
// ------------------------------------------------------------------------------------ //
scripts_api::shaderParameter_t scripts_api::MaterialProxyVar_GetValueShaderParameter( materialProxyVar_t Object )
{
	if ( !Object ) return nullptr;
	return static_cast< le::MaterialProxyVar* >( Object )->GetValueShaderParameter();
}

// ------------------------------------------------------------------------------------ //
// Get value array float
// ------------------------------------------------------------------------------------ //
float* scripts_api::MaterialProxyVar_GetValueArrayFloat( materialProxyVar_t Object, uint32_t* Count )
{
	if ( !Object )
	{
		*Count = 0;
		return nullptr;
	}

	uint32_t		count = Count ? *Count : 0;
	float*			array = static_cast< le::MaterialProxyVar* >( Object )->GetValueArrayFloat( count );

	*Count = count;
	return array;
}

// ------------------------------------------------------------------------------------ //
// Get value array int
// ------------------------------------------------------------------------------------ //
int* scripts_api::MaterialProxyVar_GetValueArrayInt( materialProxyVar_t Object, uint32_t* Count )
{
	if ( !Object )
	{
		*Count = 0;
		return nullptr;
	}

	uint32_t		count = Count ? *Count : 0;
	int*			array = static_cast< le::MaterialProxyVar* >( Object )->GetValueArrayInt( count );

	*Count = count;
	return array;
}

// ------------------------------------------------------------------------------------ //
// Get value array vec2
// ------------------------------------------------------------------------------------ //
scripts_api::vec2f_t* scripts_api::MaterialProxyVar_GetValueArrayVec2( materialProxyVar_t Object, uint32_t* Count )
{
	if ( !Object )
	{
		*Count = 0;
		return nullptr;
	}

	uint32_t				count = Count ? *Count : 0;
	le::Vector2D_t*			array = static_cast< le::MaterialProxyVar* >( Object )->GetValueArrayVector2D( count );

	*Count = count;
	return ( vec2f_t* ) array;
}

// ------------------------------------------------------------------------------------ //
// Get value array vec3
// ------------------------------------------------------------------------------------ //
scripts_api::vec3f_t* scripts_api::MaterialProxyVar_GetValueArrayVec3( materialProxyVar_t Object, uint32_t* Count )
{
	if ( !Object )
	{
		*Count = 0;
		return nullptr;
	}

	uint32_t				count = Count ? *Count : 0;
	le::Vector3D_t*			array = static_cast< le::MaterialProxyVar* >( Object )->GetValueArrayVector3D( count );

	*Count = count;
	return ( vec3f_t* ) array;
}

// ------------------------------------------------------------------------------------ //
// Get value array vec4
// ------------------------------------------------------------------------------------ //
scripts_api::vec4f_t* scripts_api::MaterialProxyVar_GetValueArrayVec4( materialProxyVar_t Object, uint32_t* Count )
{
	if ( !Object )
	{
		*Count = 0;
		return nullptr;
	}

	uint32_t				count = Count ? *Count : 0;
	le::Vector4D_t*			array = static_cast< le::MaterialProxyVar* >( Object )->GetValueArrayVector4D( count );

	*Count = count;
	return ( vec4f_t* ) array;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void scripts_api::MaterialProxyVar_Delete( materialProxyVar_t Object )
{
	if ( !Object ) return;

	le::MaterialProxyVar*		object = static_cast< le::MaterialProxyVar* >( Object );
	if ( object->GetCountReferences() <= 1 )
		object->Release();
	else
		object->DecrementReference();

	object = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Create
// ------------------------------------------------------------------------------------ //
scripts_api::materialProxyVar_t scripts_api::MaterialProxyVar_Create()
{
	return le::g_engineFactory->Create( MATERIALPROXYVAR_INTERFACE_VERSION );
}
