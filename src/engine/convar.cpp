//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "consolesystem.h"
#include "convar.h"
#include "global.h"

// ------------------------------------------------------------------------------------ //
// Инициализировать консольную переменную
// ------------------------------------------------------------------------------------ //
void le::ConVar::Initialize( const char* Name, const char* DefaultValue, CONVAR_TYPE VarType, const char* HelpString, ChangeCallbackFn_t ChangeCallback, bool IsReadOnly )
{
	name = Name;
	isReadOnly = IsReadOnly;
	changeCallback = ChangeCallback;
	defaultValue = DefaultValue;
	helpString = HelpString;

	SetValue( DefaultValue, VarType );
}

// ------------------------------------------------------------------------------------ //
// Инициализировать консольную переменную
// ------------------------------------------------------------------------------------ //
void le::ConVar::Initialize( const char* Name, const char* DefaultValue, CONVAR_TYPE VarType, const char* HelpString, bool HasMin, float Min, bool HasMax, float Max, ChangeCallbackFn_t ChangeCallback, bool IsReadOnly )
{
	name = Name;
	isReadOnly = IsReadOnly;
	changeCallback = ChangeCallback;
	defaultValue = DefaultValue;
	helpString = HelpString;
	hasMin = HasMin;
	hasMax = HasMax;
	min = Min;
	max = Max;

	SetValue( DefaultValue, VarType );
}

// ------------------------------------------------------------------------------------ //
// Вернуть значение по-умолчанию
// ------------------------------------------------------------------------------------ //
void le::ConVar::Revert()
{
	SetValue( defaultValue.c_str(), type );
}

// ------------------------------------------------------------------------------------ //
// Задать функцию смены значения
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetChangeCallback( ChangeCallbackFn_t ChangeCallback )
{
	changeCallback = ChangeCallback;
}

// ------------------------------------------------------------------------------------ //
// Задать название переменной
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetName( const char* Name )
{
	name = Name;
}

// ------------------------------------------------------------------------------------ //
// Задать минимальное значение переменной
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetMin( bool HasMin, float Min )
{
	hasMin = HasMin;
	min = Min;
}

// ------------------------------------------------------------------------------------ //
// Задать максимальное значение переменной
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetMax( bool HasMax, float Max )
{
	hasMax = HasMax;
	max = Max;
}

// ------------------------------------------------------------------------------------ //
// Задать значение
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValue( const char* Value, CONVAR_TYPE VarType )
{
	if ( isReadOnly ) return;

	try
	{
		switch ( VarType )
		{
		case CVT_INT:
			SetValueInt( std::stoi( Value ) );
			break;

		case CVT_FLOAT:
			SetValueFloat( std::stof( Value ) );
			break;

		case CVT_BOOL:
		{
			std::string			valueLower = Value;
			for ( uint32_t index = 0, count = valueLower.size(); index < count; ++index )
				valueLower[ index ] = tolower( valueLower[ index ] );

			SetValueBool( valueLower == "true" || stoi( valueLower ) ? true : false );
			break;
		}

		case CVT_STRING:
			SetValueString( Value );
			break;

		default: return;
		}
	}
	catch ( std::exception& Exception )
	{
		g_consoleSystem->PrintError( "%s: %s. Value = %s", name.c_str(), Exception.what(), Value );
	}
}

// ------------------------------------------------------------------------------------ //
// Задать значение типа int
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueInt( int Value )
{
	if ( isReadOnly ) return;

	type = CVT_INT;

	if ( hasMin && Value < min )
        values.value_int = min;
	else if ( hasMax && Value > max )
        values.value_int = max;
	else
        values.value_int = Value;

	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// Задать значение типа float
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueFloat( float Value )
{
	if ( isReadOnly ) return;

	type = CVT_FLOAT;

	if ( hasMin && Value < min )
        values.value_float = min;
	else if ( hasMax && Value > max )
        values.value_float = max;
	else
        values.value_float = Value;

	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// Задать значение типа bool
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueBool( bool Value )
{
	if ( isReadOnly ) return;

	type = CVT_BOOL;

	if ( hasMin && Value < min )
        values.value_bool = min;
	else if ( hasMax && Value > max )
        values.value_bool = max;
	else
        values.value_bool = Value;

	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// Задать значение типа строки
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueString( const char* Value )
{
	if ( isReadOnly ) return;

	type = CVT_STRING;
	std::string			value = Value;

	if ( hasMin && value.size() < min )
	{
        values.value_string = value;
        values.value_string.append( min - value.size(), ' ' );
	}
	else if ( hasMax && value.size() > max )
        values.value_string = value.substr( 0, max );
	else
        values.value_string = value;

	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// Получить название переменной
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetName() const
{
	return name.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить тип переменной
// ------------------------------------------------------------------------------------ //
le::CONVAR_TYPE le::ConVar::GetType() const
{
	return type;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
int le::ConVar::GetValueInt() const
{
    return values.value_int;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
float le::ConVar::GetValueFloat() const
{
    return values.value_float;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
bool le::ConVar::GetValueBool() const
{
    return values.value_bool;
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetValueString() const
{
    return values.value_string.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить строку-подсказку
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetHelpText() const
{
	return helpString.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить значение по-умолчанию
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetValueDefault() const
{
	return defaultValue.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить минимальное значение переменной
// ------------------------------------------------------------------------------------ //
float le::ConVar::GetMin() const
{
	return min;
}

// ------------------------------------------------------------------------------------ //
// Получить максимальное значение переменной
// ------------------------------------------------------------------------------------ //
float le::ConVar::GetMax() const
{
	return max;
}

// ------------------------------------------------------------------------------------ //
// Переменная только для чтения?
// ------------------------------------------------------------------------------------ //
bool le::ConVar::IsReadOnly() const
{
	return isReadOnly;
}

// ------------------------------------------------------------------------------------ //
// Задано ли минимальное значение
// ------------------------------------------------------------------------------------ //
bool le::ConVar::HasMin() const
{
	return hasMin;
}

// ------------------------------------------------------------------------------------ //
// Задано ли максимальное значение
// ------------------------------------------------------------------------------------ //
bool le::ConVar::HasMax() const
{
	return hasMax;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ConVar::ConVar() :
	isReadOnly( false ),
	hasMin( false ),
	hasMax( false ),
	changeCallback( nullptr ),
	min( 0.f ),
	max( 0.f ),
    type( CVT_UNDEFINED )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ConVar::~ConVar()
{}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::ConVar::Values::Values()
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ConVar::Values::~Values()
{}
