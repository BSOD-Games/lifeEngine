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
#include "engine/iconvar.h"
#include "engine/imaterial.h"
#include "settingscontext.h"
#include "common/shaderdescriptor.h"
#include "common/meshsurface.h"
#include "engine/iresourcesystem.h"
#include "global.h"
#include "studiorender.h"
#include "gpuprogram.h"
#include "texture.h"
#include "mesh.h"
#include "studiorendertechnique.h"
#include "studiorenderpass.h"
#include "openglstate.h"

LIFEENGINE_STUDIORENDER_API( le::StudioRender );

le::IConVar*		r_wireframe = nullptr;

// ------------------------------------------------------------------------------------ //
// Начать отрисовку сцены
// ------------------------------------------------------------------------------------ //
void le::StudioRender::BeginScene( ICamera* Camera )
{
	LIFEENGINE_ASSERT( Camera );

	currentScene = scenes.size();
	scenes.push_back( SceneDescriptor() );
	scenes[ currentScene ].camera = Camera;
}

// ------------------------------------------------------------------------------------ //
// Добавить меш в очередь на отрисовку
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SubmitMesh( IMesh* Mesh, const Matrix4x4_t& Transformation )
{
	LIFEENGINE_ASSERT( Mesh );
	if ( !Mesh->IsCreated() ) return;

	SubmitMesh( Mesh, Transformation, 0, Mesh->GetCountSurfaces() );
}

// ------------------------------------------------------------------------------------ //
// Добавить меш в очередь на отрисовку
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SubmitMesh( IMesh* Mesh, const Matrix4x4_t& Transformation, UInt32_t StartSurface, UInt32_t CountSurface )
{
	LIFEENGINE_ASSERT( Mesh );
	if ( !Mesh->IsCreated() )	
		return;

	le::Mesh*			mesh = ( le::Mesh* ) Mesh;
	MeshSurface*		surfaces = mesh->GetSurfaces();
	MeshSurface*		surface;	

	if ( StartSurface + CountSurface > Mesh->GetCountSurfaces() )
	{
		return;
	//	CountSurface = Mesh->GetCountSurfaces() - StartSurface;
	}

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

	for ( UInt32_t index = StartSurface, countSurfaces = StartSurface + CountSurface, maxCountSurfaces = mesh->GetCountSurfaces(); index < countSurfaces && index < maxCountSurfaces; ++index )
	{
		surface = &surfaces[ index ];

		renderObject.startVertexIndex = surface->startVertexIndex;
		renderObject.startIndex = surface->startIndex;
		renderObject.countIndeces = surface->countIndeces;
		renderObject.lightmap = ( Texture* ) mesh->GetLightmap( surface->lightmapID );
		renderObject.material = mesh->GetMaterial( surface->materialID );

		if ( !renderObject.material ) continue;
		scenes[ currentScene ].renderObjects.push_back( renderObject );
	}
}

// ------------------------------------------------------------------------------------ //
// Закончить отрисовку сцены
// ------------------------------------------------------------------------------------ //
void le::StudioRender::EndScene()
{}

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

	glEnable( GL_TEXTURE_2D );
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

	OpenGLState::Initialize();
	
	viewport.x = viewport.y = 0;
	Engine->GetWindow()->GetSize( viewport.width, viewport.height );

	// Инициализируем консольные команды
	r_wireframe = ( IConVar* ) g_consoleSystem->GetFactory()->Create( CONVAR_INTERFACE_VERSION );
	r_wireframe->Initialize( "r_wireframe", "0", CVT_BOOL, "Enable wireframe mode", true, 0, true, 1,
							 []( le::IConVar* Var )
							 {
								 if ( Var->GetValueBool() )
									 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
								 else
									 glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
							 } );

	g_consoleSystem->RegisterVar( r_wireframe );
	g_engine = Engine;

	return true;
}

// ------------------------------------------------------------------------------------ //
// Подготовить систему к отрисовке кадра
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Begin()
{
	LIFEENGINE_ASSERT( renderContext.IsCreated() );
	glViewport( viewport.x, viewport.y, viewport.width, viewport.height );

	currentScene = 0;
	scenes.clear();
}

// ------------------------------------------------------------------------------------ //
// Закончить отрисовку кадра
// ------------------------------------------------------------------------------------ //
void le::StudioRender::End()
{
	// TODO: Добавить сортировку по материалам
}

// ------------------------------------------------------------------------------------ //
// Визуализировать кадр
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Present()
{
	LIFEENGINE_ASSERT( renderContext.IsCreated() );
	glClear( GL_DEPTH_BUFFER_BIT );

	for ( UInt32_t indexScene = 0, countScenes = scenes.size(); indexScene < countScenes; ++indexScene )
	{
		SceneDescriptor&			sceneDescriptor = scenes[ indexScene ];

		for ( UInt32_t indexObject = 0, countObjects = sceneDescriptor.renderObjects.size(); indexObject < countObjects; ++indexObject )
		{
			RenderObject&			renderObject = sceneDescriptor.renderObjects[ indexObject ];
			StudioRenderTechnique*	technique = ( StudioRenderTechnique* ) renderObject.material->GetTechnique( RT_DEFFERED_SHADING );
			if ( !technique ) continue;

			for ( UInt32_t indexPass = 0, countPasses = technique->GetCountPasses(); indexPass < countPasses; ++indexPass )
			{
				StudioRenderPass*		pass = ( StudioRenderPass* ) technique->GetPass( indexPass );

				pass->Apply( renderObject.transformation, sceneDescriptor.camera, renderObject.lightmap );
				renderObject.vertexArrayObject->Bind();
				glDrawRangeElementsBaseVertex( renderObject.primitiveType, 0, renderObject.countIndeces, renderObject.countIndeces, GL_UNSIGNED_INT, ( void* ) ( renderObject.startIndex * sizeof( UInt32_t ) ), renderObject.startVertexIndex );
			}
		}
	}

	renderContext.SwapBuffers();
}

// ------------------------------------------------------------------------------------ //
// Включить вертикальную синхронизацию
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SetVerticalSyncEnabled( bool IsEnabled )
{
	LIFEENGINE_ASSERT( renderContext.IsCreated() );
	renderContext.SetVerticalSync( IsEnabled );
}

// ------------------------------------------------------------------------------------ //
// Задать порт вывода
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SetViewport( const StudioRenderViewport& Viewport )
{
	viewport = Viewport;
}

// ------------------------------------------------------------------------------------ //
// Получить фабрику системы рендера
// ------------------------------------------------------------------------------------ //
le::IFactory* le::StudioRender::GetFactory() const
{
	return ( IFactory* ) &studioRenderFactory;
}

// ------------------------------------------------------------------------------------ //
// Получить менеджер шейдеров
// ------------------------------------------------------------------------------------ //
le::IShaderManager* le::StudioRender::GetShaderManager() const
{
	return ( IShaderManager* ) &shaderManager;
}

// ------------------------------------------------------------------------------------ //
// Получить порт вывода
// ------------------------------------------------------------------------------------ //
const le::StudioRenderViewport& le::StudioRender::GetViewport() const
{
	return viewport;
}

// ------------------------------------------------------------------------------------ //
// Конструктор
// ------------------------------------------------------------------------------------ //
le::StudioRender::StudioRender() :
	isInitialize( false ),
	currentScene( 0 )
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
