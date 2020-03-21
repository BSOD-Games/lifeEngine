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
// Delete value
// ------------------------------------------------------------------------------------ //
void le::ConVar::DeleteValue()
{
    if ( type == CVT_UNDEFINED )    return;

    switch ( type )
    {
    case CVT_INT:       delete static_cast< int* >( value );            break;
    case CVT_BOOL:      delete static_cast< bool* >( value );           break;
    case CVT_FLOAT:     delete static_cast< float* >( value );          break;
    case CVT_STRING:    delete static_cast< std::string* >( value );    break;
    }

    type = CVT_UNDEFINED;
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

    if ( type != CVT_UNDEFINED && type != CVT_INT )         DeleteValue();
    if ( type == CVT_UNDEFINED )                            value = new int;

    auto        intValue = static_cast< int* >( value );

	if ( hasMin && Value < min )
        *intValue = min;
	else if ( hasMax && Value > max )
        *intValue = max;
	else
        *intValue = Value;

    type = CVT_INT;
	if ( changeCallback )		changeCallback( this );   
}

// ------------------------------------------------------------------------------------ //
// Задать значение типа float
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueFloat( float Value )
{
	if ( isReadOnly ) return;

    if ( type != CVT_UNDEFINED && type != CVT_FLOAT )       DeleteValue();
    if ( type == CVT_UNDEFINED )                            value = new float;

    auto        floatValue = static_cast< float* >( value );

	if ( hasMin && Value < min )
        *floatValue = min;
	else if ( hasMax && Value > max )
        *floatValue = max;
	else
        *floatValue = Value;

    type = CVT_FLOAT;
	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// Задать значение типа bool
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueBool( bool Value )
{
	if ( isReadOnly ) return;

    if ( type != CVT_UNDEFINED && type != CVT_BOOL )        DeleteValue();
    if ( type == CVT_UNDEFINED )                            value = new bool;

    auto        boolValue = static_cast< bool* >( value );

	if ( hasMin && Value < min )
        *boolValue = min;
	else if ( hasMax && Value > max )
        *boolValue = max;
	else
        *boolValue = Value;

    type = CVT_BOOL;
	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// Задать значение типа строки
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueString( const char* Value )
{
	if ( isReadOnly ) return;

    if ( type != CVT_UNDEFINED && type != CVT_STRING )          DeleteValue();
    if ( type == CVT_UNDEFINED )                                value = new std::string();

	std::string			value = Value;
    auto                stringValue = static_cast< std::string* >( this->value );

	if ( hasMin && value.size() < min )
	{
        *stringValue = value;
        stringValue->append( min - value.size(), ' ' );
	}
	else if ( hasMax && value.size() > max )
        *stringValue = value.substr( 0, max );
	else
        *stringValue = value;

    type = CVT_STRING;
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
    if ( type != CVT_INT )      return 0;
    return *static_cast< int* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
float le::ConVar::GetValueFloat() const
{
    if ( type != CVT_FLOAT )      return 0.f;
    return *static_cast< float* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
bool le::ConVar::GetValueBool() const
{
    if ( type != CVT_BOOL )      return false;
    return *static_cast< bool* >( value );
}

// ------------------------------------------------------------------------------------ //
// Получить значение переменной
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetValueString() const
{
    if ( type != CVT_STRING )      return "";
    return static_cast< std::string* >( value )->c_str();
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
    value( nullptr ),
	changeCallback( nullptr ),
	min( 0.f ),
	max( 0.f ),
    type( CVT_UNDEFINED ),
    countReference( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::ConVar::~ConVar()
{
    DeleteValue();
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::ConVar::IncrementReference()
{
    ++countReference;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::ConVar::DecrementReference()
{
    --countReference;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::ConVar::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ConVar::GetCountReferences() const
{
    return countReference;
}
