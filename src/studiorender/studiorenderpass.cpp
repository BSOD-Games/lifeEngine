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
#include "studiorender.h"
#include "studiorenderpass.h"
#include "shadermanager.h"

// ------------------------------------------------------------------------------------ //
// Добавить параметер
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::AddParameter( IShaderParameter* Parameter )
{
	LIFEENGINE_ASSERT( Parameter );
	
	parameters.push_back( ( ShaderParameter* ) Parameter );
	parameters.back()->SetPass( this );
	isNeadRefrash = true;
}

// ------------------------------------------------------------------------------------ //
// Удалить параметер
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::RemoveParameter( UInt32_t Index )
{
	LIFEENGINE_ASSERT( Index < parameters.size() );

	delete parameters[ Index ];
	parameters.erase( parameters.begin() + Index );
	isNeadRefrash = true;
}

// ------------------------------------------------------------------------------------ //
// Очистить проход
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::Clear()
{
	for ( UInt32_t index = 0, count = parameters.size(); index < count; ++index )
		delete parameters[ index ];

	shader = nullptr;
	parameters.clear();
}

// ------------------------------------------------------------------------------------ //
// Применить настройки прохода к рендеру
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::Apply( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	if ( shader && ( !isNeadRefrash || Refrash() ) )
		shader->OnDrawMesh( parameters.size(), ( IShaderParameter** ) parameters.data(), Transformation, Camera, Lightmap );
}

// ------------------------------------------------------------------------------------ //
// Обновить шейдер в проходе
// ------------------------------------------------------------------------------------ //
bool le::StudioRenderPass::Refrash()
{
	if ( !isNeadRefrash )		return true;

	try
	{
		if ( !shader )			throw;

		while ( !shader->InitInstance( parameters.size(), ( IShaderParameter** ) parameters.data() ) )
		{
			const char* fallbackShader = shader->GetFallbackShader();
			if ( !fallbackShader || strcmp( fallbackShader, "" ) == 0 )		throw;

			SetShader( fallbackShader );
			if ( !shader )		throw;
		}
	}
	catch ( ... )
	{
		return false;
	}

	isNeadRefrash = false;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Задать шейдер
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::SetShader( const char* NameShader )
{
	if ( !NameShader || NameShader == "" )
	{
		shader = nullptr;
		return;
	}

	ShaderManager*			shaderManager = ( ShaderManager* ) g_studioRender->GetShaderManager();
	IShader*				tempShader = shaderManager->FindShader( NameShader );
	
	if ( tempShader )
	{
		shader = tempShader;
		isNeadRefrash = true;
	}
	else
		g_consoleSystem->PrintError( "Shader [%s] not found in system", NameShader );
}

// ------------------------------------------------------------------------------------ //
// Получить шейдер
// ------------------------------------------------------------------------------------ //
const char* le::StudioRenderPass::GetNameShader() const
{
	if ( !shader )		return "";
	return shader->GetName();
}

// ------------------------------------------------------------------------------------ //
// Получить количество параметров
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::StudioRenderPass::GetCountParameters() const
{
	return parameters.size();
}

// ------------------------------------------------------------------------------------ //
// Получить все параметры
// ------------------------------------------------------------------------------------ //
le::IShaderParameter** le::StudioRenderPass::GetParameters() const
{
	return ( IShaderParameter** ) parameters.data();
}

// ------------------------------------------------------------------------------------ //
// Получить параметер
// ------------------------------------------------------------------------------------ //
le::IShaderParameter* le::StudioRenderPass::GetParameter( UInt32_t Index ) const
{
	LIFEENGINE_ASSERT( Index < parameters.size() );
	return ( IShaderParameter* ) parameters[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::StudioRenderPass::StudioRenderPass() : 
	isNeadRefrash( true ),
	shader( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::StudioRenderPass::~StudioRenderPass()
{
	Clear();
}
