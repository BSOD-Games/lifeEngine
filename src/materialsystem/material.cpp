//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/lifeengine.h"
#include "engine/iconsolesystem.h"
#include "stdshaders/ishader.h"

#include "global.h"
#include "materialsystem.h"
#include "material.h"
#include "materialvar.h"

// ------------------------------------------------------------------------------------ //
// Добавить переменную в материал
// ------------------------------------------------------------------------------------ //
void le::Material::AddVar( IMaterialVar* MaterialVar )
{
	LIFEENGINE_ASSERT( MaterialVar );
	
	std::string			varName = MaterialVar->GetName();

	for ( UInt32_t index = 0, count = vars.size(); index < count; ++index )
		if ( vars[ index ]->GetName() == varName )
		{
			g_consoleSystem->PrintError( "Material var [%s] already exist in material [%s]", varName.c_str(), name.c_str() );
			return;
		}

	vars.push_back( ( le::MaterialVar* ) MaterialVar );
}

// ------------------------------------------------------------------------------------ //
// Найти переменную
// ------------------------------------------------------------------------------------ //
le::IMaterialVar* le::Material::FindVar( const char* Name )
{
	LIFEENGINE_ASSERT( Name );

	for ( UInt32_t index = 0, count = vars.size(); index < count; ++index )
		if ( strcmp( vars[ index ]->GetName(), Name ) == 0 )
			return vars[ index ];

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Очистить материал
// ------------------------------------------------------------------------------------ //
void le::Material::Clear()
{
	shader = nullptr;
	vars.clear();
}

// ------------------------------------------------------------------------------------ //
// Задать название материала
// ------------------------------------------------------------------------------------ //
void le::Material::SetName( const char* Name )
{
	name = Name;
}

// ------------------------------------------------------------------------------------ //
// Задать шейдер
// ------------------------------------------------------------------------------------ //
void le::Material::SetShader( const char* Name )
{
	IShader*		tempShader = g_materialSystem->FindShader( Name );
	if ( !tempShader ) return;

	shader = tempShader;
}

// ------------------------------------------------------------------------------------ //
// Получить название материала
// ------------------------------------------------------------------------------------ //
const char* le::Material::GetName() const
{
	return name.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить название шейдера
// ------------------------------------------------------------------------------------ //
const char* le::Material::GetShaderName() const
{
	return shader ? shader->GetName() : "";
}

// ------------------------------------------------------------------------------------ //
// Получить количество переменных
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Material::GetCountVars() const
{
	return vars.size();
}

// ------------------------------------------------------------------------------------ //
// Получить по индексу переменную
// ------------------------------------------------------------------------------------ //
le::IMaterialVar* le::Material::GetVar( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( Index >= vars.size() );
	return ( IMaterialVar* ) vars[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Получить массив переменных
// ------------------------------------------------------------------------------------ //
le::IMaterialVar** le::Material::GetVars() const
{
	return ( IMaterialVar** ) vars.data();
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Material::Material() :
	name( "unknow" ),
	shader( nullptr )
{}
