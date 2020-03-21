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

    Technique->IncrementReference();
	technique.push_back( Technique );
}

// ------------------------------------------------------------------------------------ //
// Удалить технику
// ------------------------------------------------------------------------------------ //
void le::Material::RemoveTechnique( UInt32_t Index )
{
	LIFEENGINE_ASSERT( Index < technique.size() );
    IStudioRenderTechnique*         technique = this->technique[ Index ];

    if ( technique->GetCountReferences() <= 1 )
        technique->Release();
    else
        technique->DecrementReference();

    this->technique.erase( this->technique.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Очистить материал
// ------------------------------------------------------------------------------------ //
void le::Material::Clear()
{
	for ( UInt32_t index = 0, count = technique.size(); index < count; ++index )
    {
        IStudioRenderTechnique*         technique = this->technique[ index ];

        if ( technique->GetCountReferences() <= 1 )
            technique->Release();
        else
            technique->DecrementReference();
    }

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
    surface( "unknow" ),
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::Material::~Material()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::Material::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::Material::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::Material::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::Material::GetCountReferences() const
{
    return countReferences;
}
