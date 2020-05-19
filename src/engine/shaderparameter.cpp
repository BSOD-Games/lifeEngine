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
#include "studiorender/itexture.h"

#include "global.h"
#include "material.h"
#include "shaderparameter.h"

// ------------------------------------------------------------------------------------ //
// Очистить переменную
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::Clear()
{
    if ( !isDefined ) return;

    switch ( type )
    {
    case SPT_INT:             delete static_cast< int* >( value );               break;
    case SPT_FLOAT:           delete static_cast< float* >( value );             break;
    case SPT_SHADER_FLAG:     delete static_cast< bool* >( value );              break;
    case SPT_VECTOR_2D:       delete static_cast< Vector2D_t* >( value );        break;
    case SPT_VECTOR_3D:       delete static_cast< Vector3D_t* >( value );        break;
    case SPT_VECTOR_4D:       delete static_cast< Vector4D_t* >( value );        break;
    case SPT_MATRIX:          delete static_cast< Matrix4x4_t* >( value );       break;
    case SPT_TEXTURE:
    {
        ITexture*           texture = static_cast< ITexture* >( value );

        if ( texture->GetCountReferences() <= 1 )       texture->Release();
        else                                            texture->DecrementReference();

        break;
    }
    }

    value = nullptr;
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
    if ( isDefined && type != SPT_INT )         Clear();
    if ( !isDefined )                           value = new int;

    *static_cast< int* >( value ) = Value;
    if ( material )		material->NeadUpdateShader();

    type = SPT_INT;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueFloat( float Value )
{
    if ( isDefined && type != SPT_FLOAT )       Clear();
    if ( !isDefined )                           value = new float;

    *static_cast< float* >( value ) = Value;
    if ( material )		material->NeadUpdateShader();

    type = SPT_FLOAT;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueShaderFlag( bool Value )
{
    if ( isDefined && type != SPT_SHADER_FLAG )       Clear();
    if ( !isDefined )                                 value = new bool;

    *static_cast< bool* >( value ) = Value;
    if ( material )		material->NeadUpdateShader();

    type = SPT_SHADER_FLAG;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueVector2D( const Vector2D_t& Value )
{
    if ( isDefined && type != SPT_VECTOR_2D )       Clear();
    if ( !isDefined )                               value = new Vector2D_t();

    *static_cast< Vector2D_t* >( value ) = Value;
    if ( material )		material->NeadUpdateShader();

    type = SPT_VECTOR_2D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueVector3D( const Vector3D_t& Value )
{
    if ( isDefined && type != SPT_VECTOR_3D )       Clear();
    if ( !isDefined )                               value = new Vector3D_t();

    *static_cast< Vector3D_t* >( value ) = Value;
    if ( material )		material->NeadUpdateShader();

    type = SPT_VECTOR_3D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueVector4D( const Vector4D_t& Value )
{
    if ( isDefined && type != SPT_VECTOR_4D )       Clear();
    if ( !isDefined )                               value = new Vector4D_t();

    *static_cast< Vector4D_t* >( value ) = Value;
    if ( material )		material->NeadUpdateShader();

    type = SPT_VECTOR_4D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueMatrix( const Matrix4x4_t& Value )
{
    if ( isDefined && type != SPT_VECTOR_4D )       Clear();
    if ( !isDefined )                               value = new Matrix4x4_t();

    *static_cast< Matrix4x4_t* >( value ) = Value;
    if ( material )		material->NeadUpdateShader();

    type = SPT_MATRIX;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::SetValueTexture( ITexture* Value )
{
    if ( isDefined && type != SPT_VECTOR_4D )       Clear();

    Value->IncrementReference();
    value = Value;
    if ( material )		material->NeadUpdateShader();

    type = SPT_TEXTURE;
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
    if ( type != SPT_INT )     return 0;
    return *static_cast< int* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
float le::ShaderParameter::GetValueFloat() const
{
    if ( type != SPT_FLOAT )     return 0.f;
    return *static_cast< float* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
bool le::ShaderParameter::GetValueShaderFlag() const
{
    if ( type != SPT_SHADER_FLAG )     return false;
    return *static_cast< bool* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const le::Vector2D_t& le::ShaderParameter::GetValueVector2D() const
{
    return *static_cast< Vector2D_t* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::ShaderParameter::GetValueVector3D() const
{
    return *static_cast< Vector3D_t* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const le::Vector4D_t& le::ShaderParameter::GetValueVector4D() const
{
    return *static_cast< Vector4D_t* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const le::Matrix4x4_t& le::ShaderParameter::GetValueMatrix() const
{
    return *static_cast< Matrix4x4_t* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
le::ITexture* le::ShaderParameter::GetValueTexture() const
{
    if ( type != SPT_TEXTURE )     return nullptr;
    return static_cast< ITexture* >( value );
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ShaderParameter::ShaderParameter() :
    isDefined( false ),
	material( nullptr ),
    value( nullptr ),
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ShaderParameter::~ShaderParameter()
{
    Clear();
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::ShaderParameter::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ShaderParameter::GetCountReferences() const
{
    return countReferences;
}
