//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/iconsolesystem.h"

#include "global.h"
#include "studiorenderpass.h"
#include "shaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Очистить переменную
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::Clear()
{
	value_int = 0;
	value_float = 0.f;
	value_shaderFlag = false;
	value_texture = nullptr;
	value_matrix4x4 = Matrix4x4_t( 0.f );
	value_vector2D = Vector2D_t( 0.f );
	value_vector3D = Vector3D_t( 0.f );
	value_vector4D = Vector4D_t( 0.f );

	isDefined = false;
}

// ------------------------------------------------------------------------------------ //
// Задать название переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetName( const char* Name )
{
	name = Name;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueInt( int Value )
{
	type = SPT_INT;
	value_int = Value;
	if ( studioRenderPass )		studioRenderPass->NeadRefrash();

	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueFloat( float Value )
{
	type = SPT_FLOAT;
	value_float = Value;
	if ( studioRenderPass )		studioRenderPass->NeadRefrash();

	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueShaderFlag( bool Value )
{
	type = SPT_SHADER_FLAG;
	value_shaderFlag = Value;
	if ( studioRenderPass )		studioRenderPass->NeadRefrash();

	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueVector2D( const Vector2D_t& Value )
{
	type = SPT_VECTOR_2D;
	value_vector2D = Value;
	if ( studioRenderPass )		studioRenderPass->NeadRefrash();

	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueVector3D( const Vector3D_t& Value )
{
	type = SPT_VECTOR_3D;
	value_vector3D = Value;
	if ( studioRenderPass )		studioRenderPass->NeadRefrash();

	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueVector4D( const Vector4D_t& Value )
{
	type = SPT_VECTOR_4D;
	value_vector4D = Value;
	if ( studioRenderPass )		studioRenderPass->NeadRefrash();

	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueMatrix( const Matrix4x4_t& Value )
{
	type = SPT_MATRIX;
	value_matrix4x4 = Value;
	if ( studioRenderPass )		studioRenderPass->NeadRefrash();

	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueTexture( ITexture* Value )
{
	type = SPT_TEXTURE;
	value_texture = Value;
	if ( studioRenderPass )		studioRenderPass->NeadRefrash();

	isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Определена ли переменная
// ------------------------------------------------------------------------------------ //
bool le::ShaderParameter::IsDefined() const
{
	return isDefined;
}

// ------------------------------------------------------------------------------------ //
// Получить название переменной
// ------------------------------------------------------------------------------------ //
const char* le::ShaderParameter::GetName() const
{
	return name.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить тип переменной
// ------------------------------------------------------------------------------------ //
le::SHADER_PARAMETER_TYPE le::ShaderParameter::GetType() const
{
	return type;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
int le::ShaderParameter::GetValueInt() const
{
	return value_int;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
float le::ShaderParameter::GetValueFloat() const
{
	return value_float;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
bool le::ShaderParameter::GetValueShaderFlag() const
{
	return value_shaderFlag;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const le::Vector2D_t& le::ShaderParameter::GetValueVector2D() const
{
	return value_vector2D;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::ShaderParameter::GetValueVector3D() const
{
	return value_vector3D;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const le::Vector4D_t& le::ShaderParameter::GetValueVector4D() const
{
	return value_vector4D;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const le::Matrix4x4_t& le::ShaderParameter::GetValueMatrix() const
{
	return value_matrix4x4;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
le::ITexture* le::ShaderParameter::GetValueTexture() const
{
	return value_texture;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderParameter::ShaderParameter() :
	isDefined( false ),
	value_int( 0 ),
	value_float( 0.f ),
	value_shaderFlag( false ),
	value_matrix4x4( 0.f ),
	value_vector2D( 0.f ),
	value_vector3D( 0.f ),
	value_vector4D( 0.f ),
	value_texture( nullptr ),
	studioRenderPass( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderParameter::~ShaderParameter()
{}
