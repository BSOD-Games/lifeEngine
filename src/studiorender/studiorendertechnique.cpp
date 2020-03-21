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
#include "studiorenderpass.h"
#include "studiorendertechnique.h"

// ------------------------------------------------------------------------------------ //
// Добавить проход в технику
// ------------------------------------------------------------------------------------ //
void le::StudioRenderTechnique::AddPass( IStudioRenderPass* Pass )
{
    LIFEENGINE_ASSERT( Pass );

    Pass->IncrementReference();
    passes.push_back( ( StudioRenderPass* ) Pass );
}

// ------------------------------------------------------------------------------------ //
// Удалить проход из техники
// ------------------------------------------------------------------------------------ //
void le::StudioRenderTechnique::RemovePass( UInt32_t Index )
{
    LIFEENGINE_ASSERT( Index < passes.size() );
    StudioRenderPass*           pass = passes[ Index ];

    if ( pass->GetCountReferences() <= 1 )
        pass->Release();
    else
        pass->DecrementReference();

    passes.erase( passes.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Очистить технику
// ------------------------------------------------------------------------------------ //
void le::StudioRenderTechnique::Clear()
{
    for ( auto it = passes.begin(), itEnd = passes.end(); it != itEnd; )
        if ( (*it)->GetCountReferences() <= 1 )
        {
            (*it)->Release();

            it = passes.erase( it );
            itEnd = passes.end();
        }
        else
            ++it;
}

// ------------------------------------------------------------------------------------ //
// Задать тип техники
// ------------------------------------------------------------------------------------ //
void le::StudioRenderTechnique::SetType( RENDER_TECHNIQUE Technique )
{
    type = Technique;
}

// ------------------------------------------------------------------------------------ //
// Получить тип техники
// ------------------------------------------------------------------------------------ //
le::RENDER_TECHNIQUE le::StudioRenderTechnique::GetType() const
{
    return type;
}

// ------------------------------------------------------------------------------------ //
// Получить количество проходов
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::StudioRenderTechnique::GetCountPasses() const
{
    return passes.size();
}

// ------------------------------------------------------------------------------------ //
// Получить все проходы
// ------------------------------------------------------------------------------------ //
le::IStudioRenderPass** le::StudioRenderTechnique::GetPasses() const
{
    return ( IStudioRenderPass** ) passes.data();
}

// ------------------------------------------------------------------------------------ //
// Получить проход
// ------------------------------------------------------------------------------------ //
le::IStudioRenderPass* le::StudioRenderTechnique::GetPass( UInt32_t Index ) const
{
    LIFEENGINE_ASSERT( Index < passes.size() );
    return ( IStudioRenderPass* ) passes[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::StudioRenderTechnique::StudioRenderTechnique() :
    countReferences( 0 )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::StudioRenderTechnique::~StudioRenderTechnique()
{
    Clear();
}

// ------------------------------------------------------------------------------------ //
// Increment reference
// ------------------------------------------------------------------------------------ //
void le::StudioRenderTechnique::IncrementReference()
{
    ++countReferences;
}

// ------------------------------------------------------------------------------------ //
// Decrement reference
// ------------------------------------------------------------------------------------ //
void le::StudioRenderTechnique::DecrementReference()
{
    --countReferences;
}

// ------------------------------------------------------------------------------------ //
// Delete
// ------------------------------------------------------------------------------------ //
void le::StudioRenderTechnique::Release()
{
    delete this;
}

// ------------------------------------------------------------------------------------ //
// Get count references
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::StudioRenderTechnique::GetCountReferences() const
{
    return  countReferences;
}
