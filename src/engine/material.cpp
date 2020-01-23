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
#include "engine/ifactory.h"
#include "studiorender/istudiorender.h"
#include "studiorender/istudiorendertechnique.h"
#include "global.h"
#include "material.h"

// ------------------------------------------------------------------------------------ //
// Добавить технику
// ------------------------------------------------------------------------------------ //
void le::Material::AddTechnique( IStudioRenderTechnique* Technique )
{
	LIFEENGINE_ASSERT( Technique );
	
	RENDER_TECHNIQUE		typeTechnique = Technique->GetType();
	for ( UInt32_t index = 0, count = technique.size(); index < count; ++index )
		if ( technique[ index ]->GetType() == typeTechnique )
			return;

	technique.push_back( Technique );
}

// ------------------------------------------------------------------------------------ //
// Удалить технику
// ------------------------------------------------------------------------------------ //
void le::Material::RemoveTechnique( UInt32_t Index )
{
	LIFEENGINE_ASSERT( Index < technique.size() );
	
	g_studioRender->GetFactory()->Delete( technique[ Index ] );
	technique.erase( technique.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Очистить материал
// ------------------------------------------------------------------------------------ //
void le::Material::Clear()
{
	for ( UInt32_t index = 0, count = technique.size(); index < count; ++index )
		g_studioRender->GetFactory()->Delete( technique[ index ] );

	surface = "unknow";
	technique.clear();
}

// ------------------------------------------------------------------------------------ //
// Задать название поверхности
// ------------------------------------------------------------------------------------ //
void le::Material::SetSurfaceName( const char* Name )
{
	surface = Name;
}

// ------------------------------------------------------------------------------------ //
// Получить название поверхности
// ------------------------------------------------------------------------------------ //
const char* le::Material::GetSurfaceName() const
{
	return surface.c_str();
}

// ------------------------------------------------------------------------------------ //
// Получить количество техник
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Material::GetCountTechiques() const
{
	return technique.size();
}

// ------------------------------------------------------------------------------------ //
// Получить массив техник
// ------------------------------------------------------------------------------------ //
le::IStudioRenderTechnique** le::Material::GetTechiques() const
{
	return ( IStudioRenderTechnique** ) technique.data();
}

// ------------------------------------------------------------------------------------ //
// Получить технику
// ------------------------------------------------------------------------------------ //
le::IStudioRenderTechnique* le::Material::GetTechnique( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( Index < technique.size() );
	return technique[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Получить технику
// ------------------------------------------------------------------------------------ //
le::IStudioRenderTechnique* le::Material::GetTechnique( RENDER_TECHNIQUE Type ) const
{
	for ( UInt32_t index = 0, count = technique.size(); index < count; ++index )
		if ( technique[ index ]->GetType() == Type )
			return technique[ index ];

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::Material::Material() :
	surface( "unknow" )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Material::~Material()
{
	Clear();
}