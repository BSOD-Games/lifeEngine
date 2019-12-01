//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "consolesystem.h"
#include "convar.h"
#include "global.h"

// ------------------------------------------------------------------------------------ //
// ���������������� ���������� ����������
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
// ���������������� ���������� ����������
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
// ������� �������� ��-���������
// ------------------------------------------------------------------------------------ //
void le::ConVar::Revert()
{
	SetValue( defaultValue.c_str(), type );
}

// ------------------------------------------------------------------------------------ //
// ������ ������� ����� ��������
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetChangeCallback( ChangeCallbackFn_t ChangeCallback )
{
	changeCallback = ChangeCallback;
}

// ------------------------------------------------------------------------------------ //
// ������ �������� ����������
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetName( const char* Name )
{
	name = Name;
}

// ------------------------------------------------------------------------------------ //
// ������ ����������� �������� ����������
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetMin( bool HasMin, float Min )
{
	hasMin = HasMin;
	min = Min;
}

// ------------------------------------------------------------------------------------ //
// ������ ������������ �������� ����������
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetMax( bool HasMax, float Max )
{
	hasMax = HasMax;
	max = Max;
}

// ------------------------------------------------------------------------------------ //
// ������ ��������
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
// ������ �������� ���� int
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueInt( int Value )
{
	if ( isReadOnly ) return;

	type = CVT_INT;

	if ( hasMin && Value < min )
		value_int = min;
	else if ( hasMax && Value > max )
		value_int = max;
	else
		value_int = Value;

	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// ������ �������� ���� float
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueFloat( float Value )
{
	if ( isReadOnly ) return;

	type = CVT_FLOAT;

	if ( hasMin && Value < min )
		value_float = min;
	else if ( hasMax && Value > max )
		value_float = max;
	else
		value_float = Value;

	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// ������ �������� ���� bool
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueBool( bool Value )
{
	if ( isReadOnly ) return;

	type = CVT_BOOL;

	if ( hasMin && Value < min )
		value_bool = min;
	else if ( hasMax && Value > max )
		value_bool = max;
	else
		value_bool = Value;

	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// ������ �������� ���� ������
// ------------------------------------------------------------------------------------ //
void le::ConVar::SetValueString( const char* Value )
{
	if ( isReadOnly ) return;

	type = CVT_STRING;
	std::string			value = Value;

	if ( hasMin && value.size() < min )
	{
		value_string = value;
		value_string.append( min - value.size(), ' ' );
	}
	else if ( hasMax && value.size() > max )
		value_string = value.substr( 0, max );
	else
		value_string = value;

	if ( changeCallback )		changeCallback( this );
}

// ------------------------------------------------------------------------------------ //
// �������� �������� ����������
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetName() const
{
	return name.c_str();
}

// ------------------------------------------------------------------------------------ //
// �������� ��� ����������
// ------------------------------------------------------------------------------------ //
le::CONVAR_TYPE le::ConVar::GetType() const
{
	return type;
}

// ------------------------------------------------------------------------------------ //
// �������� �������� ����������
// ------------------------------------------------------------------------------------ //
int le::ConVar::GetValueInt() const
{
	return value_int;
}

// ------------------------------------------------------------------------------------ //
// �������� �������� ����������
// ------------------------------------------------------------------------------------ //
float le::ConVar::GetValueFloat() const
{
	return value_float;
}

// ------------------------------------------------------------------------------------ //
// �������� �������� ����������
// ------------------------------------------------------------------------------------ //
bool le::ConVar::GetValueBool() const
{
	return value_bool;
}

// ------------------------------------------------------------------------------------ //
// �������� �������� ����������
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetValueString() const
{
	return value_string.c_str();
}

// ------------------------------------------------------------------------------------ //
// �������� ������-���������
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetHelpText() const
{
	return helpString.c_str();
}

// ------------------------------------------------------------------------------------ //
// �������� �������� ��-���������
// ------------------------------------------------------------------------------------ //
const char* le::ConVar::GetValueDefault() const
{
	return defaultValue.c_str();
}

// ------------------------------------------------------------------------------------ //
// �������� ����������� �������� ����������
// ------------------------------------------------------------------------------------ //
float le::ConVar::GetMin() const
{
	return min;
}

// ------------------------------------------------------------------------------------ //
// �������� ������������ �������� ����������
// ------------------------------------------------------------------------------------ //
float le::ConVar::GetMax() const
{
	return max;
}

// ------------------------------------------------------------------------------------ //
// ���������� ������ ��� ������?
// ------------------------------------------------------------------------------------ //
bool le::ConVar::IsReadOnly() const
{
	return isReadOnly;
}

// ------------------------------------------------------------------------------------ //
// ������ �� ����������� ��������
// ------------------------------------------------------------------------------------ //
bool le::ConVar::HasMin() const
{
	return hasMin;
}

// ------------------------------------------------------------------------------------ //
// ������ �� ������������ ��������
// ------------------------------------------------------------------------------------ //
bool le::ConVar::HasMax() const
{
	return hasMax;
}

// ------------------------------------------------------------------------------------ //
// �����������
// ------------------------------------------------------------------------------------ //
le::ConVar::ConVar() :
	isReadOnly( false ),
	hasMin( false ),
	hasMax( false ),
	changeCallback( nullptr ),
	min( 0.f ),
	max( 0.f ),
	type( CVT_UNDEFINED ),
	value_int( 0 ),
	value_float( 0.f ),
	value_bool( false ),
	value_string( "" )
{}

// ------------------------------------------------------------------------------------ //
// ����������
// ------------------------------------------------------------------------------------ //
le::ConVar::~ConVar()
{}
