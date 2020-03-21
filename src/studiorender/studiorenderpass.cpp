//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "engine/lifeengine.h"
#include "engine/iconsolesystem.h"
#include "engine/imaterialproxy.h"
#include "engine/iengine.h"
#include "stdshaders/ishader.h"
#include "global.h"
#include "studiorender.h"
#include "studiorenderpass.h"
#include "shadermanager.h"
#include "openglstate.h"

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
    materialProxes.clear();
}

// ------------------------------------------------------------------------------------ //
// Применить настройки прохода к рендеру
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::Apply( const Matrix4x4_t& Transformation, ICamera* Camera, ITexture* Lightmap )
{
	InitStates();

	if ( shader && ( !isNeadRefrash || Refrash() ) )
    {
        for ( UInt32_t index = 0, count = materialProxes.size(); index < count; ++index )
            materialProxes[ index ]->NeadUpdate();

		shader->OnDrawMesh( parameters.size(), ( IShaderParameter** ) parameters.data(), Transformation, Camera, Lightmap );
    }
}

// ------------------------------------------------------------------------------------ //
// Инициализировать состаяния OpenGL
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::InitStates()
{
	OpenGLState::EnableDepthTest( isDepthTest );
	OpenGLState::EnableDepthWrite( isDepthWrite );
	OpenGLState::EnableBlend( isBlend );
	OpenGLState::EnableCullFace( isCullFace );
	OpenGLState::SetCullFaceType( cullFaceType );
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
// Включить ли тест глубины
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::EnableDepthTest( bool Enable )
{
	isDepthTest = Enable;
}

// ------------------------------------------------------------------------------------ //
// Включить ли запись в буфер глубины
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::EnableDepthWrite( bool Enable )
{
	isDepthWrite = Enable;
}

// ------------------------------------------------------------------------------------ //
// Включить ли смешивание
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::EnableBlend( bool Enable )
{
	isBlend = Enable;
}

// ------------------------------------------------------------------------------------ //
// Включить ли отсечение полигонов
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::EnableCullFace( bool Enable )
{
    isCullFace = Enable;
}

// ------------------------------------------------------------------------------------ //
// Включить ли отсечение полигонов
// ------------------------------------------------------------------------------------ //
le::IShaderParameter* le::StudioRenderPass::FindParameter( const char* Name ) const
{
    if ( parameters.empty() )       return nullptr;

    for ( UInt32_t index = 0, count = parameters.size(); index < count; ++index )
        if ( strcmp( Name, parameters[ index ]->GetName() ) == 0 )
            return parameters[ index ];

    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Задать тип отсикаемых полигонов
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::SetCullFaceType( CULLFACE_TYPE CullFaceType )
{
	cullFaceType = CullFaceType;
}

// ------------------------------------------------------------------------------------ //
// Включен ли тест глубины
// ------------------------------------------------------------------------------------ //
bool le::StudioRenderPass::IsDepthTest() const
{
	return isDepthTest;
}

// ------------------------------------------------------------------------------------ //
// Включена ли запись в буфер глубины
// ------------------------------------------------------------------------------------ //
bool le::StudioRenderPass::IsDepthWrite() const
{
	return isDepthWrite;
}

// ------------------------------------------------------------------------------------ //
// Включено ли смешивание
// ------------------------------------------------------------------------------------ //
bool le::StudioRenderPass::IsBlend() const
{
	return isBlend;
}

// ------------------------------------------------------------------------------------ //
// Включено ли отсечение полигонов
// ------------------------------------------------------------------------------------ //
bool le::StudioRenderPass::IsCullFace() const
{
	return isCullFace;
}

// ------------------------------------------------------------------------------------ //
// Получить тип отсикаемых полигонов
// ------------------------------------------------------------------------------------ //
le::CULLFACE_TYPE le::StudioRenderPass::GetCullFaceType() const
{
	return cullFaceType;
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
	isDepthTest( true ),
	isDepthWrite( true ),
	isBlend( false ),
	isCullFace( true ),
	cullFaceType( CT_BACK ),
	shader( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::StudioRenderPass::~StudioRenderPass()
{
	Clear();
}

// ------------------------------------------------------------------------------------ //
// Добавить прокси-материал в проход
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::AddProxy( IMaterialProxy* MaterialProxy )
{
    LIFEENGINE_ASSERT( MaterialProxy );
    materialProxes.push_back( MaterialProxy );
}

// ------------------------------------------------------------------------------------ //
// Удалить прокси-материал из прохода
// ------------------------------------------------------------------------------------ //
void le::StudioRenderPass::RemoveProxy( UInt32_t Index )
{
    LIFEENGINE_ASSERT( Index < materialProxes.size() );
    materialProxes.erase( materialProxes.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Получить количество прокси-материалов
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::StudioRenderPass::GetCountProxes() const
{
    return materialProxes.size();
}

// ------------------------------------------------------------------------------------ //
// Получить массив прокси-материалов
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy** le::StudioRenderPass::GetProxes() const
{
    return ( IMaterialProxy** ) materialProxes.data();
}

// ------------------------------------------------------------------------------------ //
// Получить прокси-материавл
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy* le::StudioRenderPass::GetProxy( UInt32_t Index ) const
{
    LIFEENGINE_ASSERT( Index < materialProxes.size() );
    return ( IMaterialProxy* ) materialProxes[ Index ];
}
