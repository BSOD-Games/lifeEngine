//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>

#include "common/configurations.h"
#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/iwindow.h"
#include "engine/iconsolesystem.h"
#include "materialsystem/imaterialinternal.h"
#include "settingscontext.h"
#include "common/shaderdescriptor.h"
#include "common/meshsurface.h"
#include "engine/iresourcesystem.h"
#include "global.h"
#include "studiorender.h"
#include "gpuprogram.h"
#include "texture.h"
#include "mesh.h"

LIFEENGINE_STUDIORENDER_API( le::StudioRender );

// ------------------------------------------------------------------------------------ //
// Изменить размер вьюпорта
// ------------------------------------------------------------------------------------ //
void le::StudioRender::ResizeViewport( UInt32_t X, UInt32_t Y, UInt32_t Width, UInt32_t Height )
{
	if ( !renderContext.IsCreated() ) return;
	glViewport( X, Y, Width, Height );
}

// ------------------------------------------------------------------------------------ //
// Добавить модель в очередь на отрисовку
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Draw( IMesh* Mesh, const Matrix4x4_t& Transformation, ICamera* Camera )
{
	LIFEENGINE_ASSERT( Mesh );

	if ( !Mesh->IsCreated() ) return;

	le::Mesh*			mesh = ( le::Mesh* ) Mesh;
	IMaterial**			materials = mesh->GetMaterials();
	MeshSurface*		surfaces = mesh->GetSurfaces();
	MeshSurface*		surface;

	RenderObject		renderObject;
	renderObject.vertexArrayObject = ( VertexArrayObject* ) &mesh->GetVertexArrayObject();
	renderObject.transformation = Transformation;

	switch ( mesh->GetPrimitiveType() )
	{
	case PT_LINES:
		renderObject.primitiveType = GL_LINE;
		break;

	case PT_TRIANGLES:
		renderObject.primitiveType = GL_TRIANGLES;
		break;
	}

	for ( UInt32_t index = 0, countSurfaces = mesh->GetCountSurfaces(), countMaterials = mesh->GetCountMaterials(); index < countSurfaces; ++index )
	{
		surface = &surfaces[ index ];
		if ( surface->materialID < 0 || surface->materialID > countMaterials )
			continue;

		renderObject.startIndex = surface->startIndex;
		renderObject.countIndeces = surface->countIndeces;
		renderObject.material = ( IMaterialInternal* ) materials[ surface->materialID ];

		renderObjects[ Camera ].push_back( renderObject );
	}
}

// ------------------------------------------------------------------------------------ //
// Инициализировать рендер
// ------------------------------------------------------------------------------------ //
bool le::StudioRender::Initialize( IEngine* Engine )
{
	if ( isInitialize ) return true;

	g_consoleSystem = Engine->GetConsoleSystem();
	if ( !g_consoleSystem )
	{
		g_consoleSystem->PrintError( "Studiorender requared console system" );
		return false;
	}

	// Если в ядре окно не создано (указатель на IWindow nullptr) или
	// заголовок окна nullptr, то выбрасываем ошибку

	if ( !Engine->GetWindow() || !Engine->GetWindow()->GetHandle() )
	{
		g_consoleSystem->PrintError( "Window not open or not valid handle" );
		return false;
	}

	// Создаем контекст OpenGL

	Configurations				configurations = Engine->GetConfigurations();
	SettingsContext				settingsContext;
	settingsContext.redBits = 8;
	settingsContext.greenBits = 8;
	settingsContext.blueBits = 8;
	settingsContext.alphaBits = 8;
	settingsContext.depthBits = 24;
	settingsContext.stencilBits = 8;
	settingsContext.majorVersion = 3;
	settingsContext.minorVersion = 3;
	settingsContext.attributeFlags = SettingsContext::CA_CORE;

	if ( !renderContext.Create( Engine->GetWindow()->GetHandle(), settingsContext ) )
	{
		g_consoleSystem->PrintError( "Failed created context" );
		return false;
	}

	renderContext.SetVerticalSync( configurations.isVerticalSinc );

	// Инициализируем OpenGL

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	return true;
}

// ------------------------------------------------------------------------------------ //
// Отрисовать кадр
// ------------------------------------------------------------------------------------ //
void le::StudioRender::RenderFrame()
{
	if ( !renderContext.IsCreated() ) return;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// TODO: Поменять метод отрисовки геометрии, ибо сейчас просадка FPS
	for ( auto it = renderObjects.begin(), itEnd = renderObjects.end(); it != itEnd; ++it )
		for ( UInt32_t index = 0, count = it->second.size(); index < count; ++index )
		{
			RenderObject* renderObject = &it->second[ index ];

			renderObject->material->OnDrawElements( renderObject->transformation, it->first );
			renderObject->vertexArrayObject->Bind();
			glDrawElements( renderObject->primitiveType, renderObject->countIndeces, GL_UNSIGNED_INT, ( void* ) ( renderObject->startIndex * sizeof( UInt32_t ) ) );
		}

	renderContext.SwapBuffers();
	renderObjects.clear();
}

// ------------------------------------------------------------------------------------ //
// Включить вертикальную синхронизацию
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SetVerticalSyncEnabled( bool IsEnabled )
{
	if ( !renderContext.IsCreated() ) return;
	renderContext.SetVerticalSync( IsEnabled );
}

// ------------------------------------------------------------------------------------ //
// Получить фабрику системы рендера
// ------------------------------------------------------------------------------------ //
le::IFactory* le::StudioRender::GetFactory() const
{
	return ( IFactory* ) &studioRenderFactory;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::StudioRender::StudioRender() :
	isInitialize( false )
{
	LIFEENGINE_ASSERT( !g_studioRender );
	g_studioRender = this;
}

// ------------------------------------------------------------------------------------ //
// Деструктор
// ------------------------------------------------------------------------------------ //
le::StudioRender::~StudioRender()
{
	if ( renderContext.IsCreated() )		renderContext.Destroy();
}
