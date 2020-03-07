//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "materialproxyvar.h"

// ------------------------------------------------------------------------------------ //
// Очистить переменную
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::Clear()
{
    values.value_int = 0;
    values.value_float = 0.f;
    values.value_bool = false;
    values.value_vector2D = Vector2D_t( 0.f );
    values.value_vector3D = Vector3D_t( 0.f );
    values.value_vector4D = Vector4D_t( 0.f );
    values.value_arrayInt.clear();
    values.value_arrayFloat.clear();
    values.value_arrayVector2D.clear();
    values.value_arrayVector3D.clear();
    values.value_arrayVector4D.clear();
    values.value_shaderParameter = nullptr;

    isDefined = false;
}

//------------------------------------------------------------------------------------ //
// Задать имя переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetName( const char* Name )
{
    name = Name;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueInt( int Value )
{
    values.value_int = Value;
    type = MPVT_INT;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueFloat( float Value )
{
    values.value_float = Value;
    type = MPVT_FLOAT;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueBool( bool Value )
{
    values.value_bool = Value;
    type = MPVT_BOOL;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueVector2D( const Vector2D_t& Value )
{
    values.value_vector2D = Value;
    type = MPVT_VECTOR_2D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueVector3D( const Vector3D_t& Value )
{
    values.value_vector3D = Value;
    type = MPVT_VECTOR_3D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueVector4D( const Vector4D_t& Value )
{
    values.value_vector4D = Value;
    type = MPVT_VECTOR_4D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueShaderParameter( IShaderParameter* Value )
{
    values.value_shaderParameter = Value;
    type = MPVT_SHADER_PARAMETER;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueArrayFloat( float* Array, UInt32_t Count )
{
    values.value_arrayFloat = { Array, Array + Count };
    type = MPVT_ARRAY_FLOAT;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueArrayInt( int* Array, UInt32_t Count )
{
    values.value_arrayInt = { Array, Array + Count };
    type = MPVT_ARRAY_INT;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueArrayVector2D( Vector2D_t* Array, UInt32_t Count )
{
    values.value_arrayVector2D = { Array, Array + Count };
    type = MPVT_ARRAY_VECTOR_2D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueArrayVector3D( Vector3D_t* Array, UInt32_t Count )
{
    values.value_arrayVector3D = { Array, Array + Count };
    type = MPVT_ARRAY_VECTOR_3D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Задать значение переменной
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyVar::SetValueArrayVector4D( Vector4D_t* Array, UInt32_t Count )
{
    values.value_arrayVector4D = { Array, Array + Count };
    type = MPVT_ARRAY_VECTOR_4D;
    isDefined = true;
}

// ------------------------------------------------------------------------------------ //
// Определена ли переменна
// ------------------------------------------------------------------------------------ //
bool le::MaterialProxyVar::IsDefined() const
{
    return isDefined;
}

// ------------------------------------------------------------------------------------ //
// Получить имя переменной
// ------------------------------------------------------------------------------------ //
const char* le::MaterialProxyVar::GetName() const
{
    return name.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить тип переменной
// ------------------------------------------------------------------------------------ //
le::MATERIAL_PROXY_VAR_TYPE le::MaterialProxyVar::GetType() const
{
    return type;
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
int le::MaterialProxyVar::GetValueInt() const
{
    return values.value_int;
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
float le::MaterialProxyVar::GetValueFloat() const
{
    return values.value_float;
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
bool le::MaterialProxyVar::GetValueBool() const
{
    return values.value_bool;
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
const le::Vector2D_t& le::MaterialProxyVar::GetValueVector2D() const
{
    return values.value_vector2D;
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
const le::Vector3D_t& le::MaterialProxyVar::GetValueVector3D() const
{
    return values.value_vector3D;
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
const le::Vector4D_t& le::MaterialProxyVar::GetValueVector4D() const
{
    return values.value_vector4D;
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
le::IShaderParameter* le::MaterialProxyVar::GetValueShaderParameter() const
{
    return values.value_shaderParameter;
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
float* le::MaterialProxyVar::GetValueArrayFloat( UInt32_t& Count )
{
    Count = values.value_arrayFloat.size();
    return values.value_arrayFloat.data();
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
int* le::MaterialProxyVar::GetValueArrayInt( UInt32_t& Count )
{
    Count = values.value_arrayInt.size();
    return values.value_arrayInt.data();
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
le::Vector2D_t* le::MaterialProxyVar::GetValueArrayVector2D( UInt32_t& Count )
{
    Count = values.value_arrayVector2D.size();
    return values.value_arrayVector2D.data();
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
le::Vector3D_t* le::MaterialProxyVar::GetValueArrayVector3D( UInt32_t& Count )
{
    Count = values.value_arrayVector3D.size();
    return values.value_arrayVector3D.data();
}

// ------------------------------------------------------------------------------------ //
// Получить переменную
// ------------------------------------------------------------------------------------ //
le::Vector4D_t* le::MaterialProxyVar::GetValueArrayVector4D( UInt32_t& Count )
{
    Count = values.value_arrayVector4D.size();
    return values.value_arrayVector4D.data();
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::MaterialProxyVar::MaterialProxyVar() :
    isDefined( false )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::MaterialProxyVar::~MaterialProxyVar()
{}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::MaterialProxyVar::Values::Values()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::MaterialProxyVar::Values::~Values()
{}
