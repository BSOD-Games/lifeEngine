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
#include "pointlight.h"
#include "spotlight.h"
#include "directionallight.h"
#include "common/meshdescriptor.h"
#include "studiorender/studiovertexelement.h"
#include "common/shaderdescriptor.h"
#include "engine/iconcmd.h"
#include "engine/icamera.h"

LIFEENGINE_STUDIORENDER_API( le::StudioRender );

le::IConVar*		r_wireframe = nullptr;
le::IConVar*		r_showgbuffer = nullptr;

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
// Добавить источник света на отрисовку
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SubmitLight( IPointLight* PointLight )
{
	LIFEENGINE_ASSERT( PointLight );
	scenes[ currentScene ].pointLights.push_back( ( le::PointLight* ) PointLight );
}

// ------------------------------------------------------------------------------------ //
// Добавить источник света на отрисовку
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SubmitLight( ISpotLight* SpotLight )
{
	LIFEENGINE_ASSERT( SpotLight );
	scenes[ currentScene ].spotLights.push_back( ( le::SpotLight* ) SpotLight );
}

// ------------------------------------------------------------------------------------ //
// Добавить источник света на отрисовку
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SubmitLight( IDirectionalLight* DirectionalLight )
{
	LIFEENGINE_ASSERT( DirectionalLight );
	scenes[ currentScene ].directionalLights.push_back( ( le::DirectionalLight* ) DirectionalLight );
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
	OpenGLState::Initialize();
	
	viewport.x = viewport.y = 0;
	Engine->GetWindow()->GetSize( viewport.width, viewport.height );

	if ( !gbuffer.Initialize( Vector2DInt_t( viewport.width, viewport.height ) ) )
	{
		g_consoleSystem->PrintError( "Failed initialize GBuffer" );
		return false;
	}

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

	r_showgbuffer = ( IConVar* ) g_consoleSystem->GetFactory()->Create( CONVAR_INTERFACE_VERSION );
	r_showgbuffer->Initialize( "r_showgbuffer", "0", CVT_BOOL, "Enable view GBuffer", true, 0, true, 1, nullptr );

	g_consoleSystem->RegisterVar( r_wireframe );
	g_consoleSystem->RegisterVar( r_showgbuffer );
	g_engine = Engine;

	quad.Create();
	sphere.Create();
	cone.Create();

	if ( !shaderDepth.Create() || !shaderLighting.Create() )
		return false;

	shaderLighting.SetType( ShaderLighting::LT_POINT );
	shaderLighting.SetSizeViewport( Vector2D_t( viewport.width, viewport.height ) );

	shaderLighting.SetType( ShaderLighting::LT_SPOT );
	shaderLighting.SetSizeViewport( Vector2D_t( viewport.width, viewport.height ) );

	shaderLighting.SetType( ShaderLighting::LT_DIRECTIONAL );
	shaderLighting.SetSizeViewport( Vector2D_t( viewport.width, viewport.height ) );

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

	for ( UInt32_t indexScene = 0, countScenes = scenes.size(); indexScene < countScenes; ++indexScene )
	{
		SceneDescriptor&			sceneDescriptor = scenes[ indexScene ];

		// Геометрический проход Deffered Shading'a
		Render_GeometryPass( sceneDescriptor );

		// Проход освещения
		Render_LightPass( sceneDescriptor );

		// Показать финальный кадр
		Render_FinalPass( sceneDescriptor );
	}

	if ( r_showgbuffer->GetValueBool() )		gbuffer.ShowBuffers();
	renderContext.SwapBuffers();
}

// ------------------------------------------------------------------------------------ //
// Геометрический проход Deffered Shading'a
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Render_GeometryPass( const SceneDescriptor& SceneDescriptor ) 
{
	gbuffer.Bind( GBuffer::BT_GEOMETRY );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for ( UInt32_t indexObject = 0, countObjects = SceneDescriptor.renderObjects.size(); indexObject < countObjects; ++indexObject )
	{
		const RenderObject&		renderObject = SceneDescriptor.renderObjects[ indexObject ];
		StudioRenderTechnique*	technique = ( StudioRenderTechnique* ) renderObject.material->GetTechnique( RT_DEFFERED_SHADING );
		if ( !technique ) continue;

		for ( UInt32_t indexPass = 0, countPasses = technique->GetCountPasses(); indexPass < countPasses; ++indexPass )
		{
			StudioRenderPass*		pass = ( StudioRenderPass* ) technique->GetPass( indexPass );

			pass->Apply( renderObject.transformation, SceneDescriptor.camera, renderObject.lightmap );
			renderObject.vertexArrayObject->Bind();
			glDrawRangeElementsBaseVertex( renderObject.primitiveType, 0, renderObject.countIndeces, renderObject.countIndeces, GL_UNSIGNED_INT, ( void* ) ( renderObject.startIndex * sizeof( UInt32_t ) ), renderObject.startVertexIndex );
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Световой проход Deffered Shading'a
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Render_LightPass( const SceneDescriptor& SceneDescriptor ) 
{
	gbuffer.Bind( GBuffer::BT_LIGHT ); 
	glClear( GL_COLOR_BUFFER_BIT );

	OpenGLState::EnableStencilTest( true );
	OpenGLState::EnableDepthWrite( false );
	OpenGLState::SetCullFaceType( CT_FRONT );
	OpenGLState::SetStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	OpenGLState::SetStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP );

	shaderDepth.SetType( ShaderDepth::GT_SPHERE );
	shaderLighting.SetType( ShaderLighting::LT_POINT );
	shaderLighting.SetCamera( SceneDescriptor.camera );
	sphere.Bind();

	for ( UInt32_t indexLight = 0, countLights = SceneDescriptor.pointLights.size(); indexLight < countLights; ++indexLight )
	{
		auto*			light = SceneDescriptor.pointLights[ indexLight ];

		OpenGLState::SetColorMask( false, false, false, false );
		OpenGLState::EnableDepthTest( true );
		glClear( GL_STENCIL_BUFFER_BIT );
		OpenGLState::SetStencilFunc( GL_ALWAYS, 0, 0 );

		shaderDepth.SetPVTMatrix( SceneDescriptor.camera->GetProjectionMatrix() * SceneDescriptor.camera->GetViewMatrix() * light->GetTransformation() );
		shaderDepth.SetRadius( light->GetRadius() );
		shaderDepth.Bind();
		glDrawElements( GL_TRIANGLES, sphere.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( sphere.GetStartIndex() * sizeof( UInt32_t ) ) );
	
		shaderLighting.SetPVTMatrix( SceneDescriptor.camera->GetProjectionMatrix() * SceneDescriptor.camera->GetViewMatrix() * light->GetTransformation() );
		shaderLighting.SetLight( light );
		shaderLighting.Bind();	
		
		OpenGLState::SetColorMask( true, true, true, true );
		OpenGLState::SetStencilFunc( GL_NOTEQUAL, 0, 0xFF );
		OpenGLState::EnableDepthTest( false );

		OpenGLState::EnableBlend( true );
		glDrawElements( GL_TRIANGLES, sphere.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( sphere.GetStartIndex() * sizeof( UInt32_t ) ) );
		OpenGLState::EnableBlend( false );
	}

	shaderDepth.SetType( ShaderDepth::GT_CONE );
	shaderLighting.SetType( ShaderLighting::LT_SPOT );
	shaderLighting.SetCamera( SceneDescriptor.camera );
	cone.Bind();

	for ( UInt32_t indexLight = 0, countLights = SceneDescriptor.spotLights.size(); indexLight < countLights; ++indexLight )
	{
		auto*			light = SceneDescriptor.spotLights[ indexLight ];

		OpenGLState::SetColorMask( false, false, false, false );
		OpenGLState::EnableDepthTest( true );
		glClear( GL_STENCIL_BUFFER_BIT );
		OpenGLState::SetStencilFunc( GL_ALWAYS, 0, 0 );
	
		shaderDepth.SetPVTMatrix( SceneDescriptor.camera->GetProjectionMatrix() * SceneDescriptor.camera->GetViewMatrix() * light->GetTransformation() );
		shaderDepth.SetRadius( light->GetRadius() );
		shaderDepth.SetHeight( light->GetHeight() );
		shaderDepth.Bind();
		glDrawElements( GL_TRIANGLES, cone.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( cone.GetStartIndex() * sizeof( UInt32_t ) ) );
	
		shaderLighting.SetPVTMatrix( SceneDescriptor.camera->GetProjectionMatrix() * SceneDescriptor.camera->GetViewMatrix() * light->GetTransformation() );
		shaderLighting.SetLight( light );
		shaderLighting.Bind();	
		
		OpenGLState::SetColorMask( true, true, true, true );
		OpenGLState::SetStencilFunc( GL_NOTEQUAL, 0, 0xFF );
		OpenGLState::EnableDepthTest( false );

		OpenGLState::EnableBlend( true );
		glDrawElements( GL_TRIANGLES, cone.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( cone.GetStartIndex() * sizeof( UInt32_t ) ) );
		OpenGLState::EnableBlend( false );
	}

	shaderLighting.SetType( ShaderLighting::LT_DIRECTIONAL );
	shaderLighting.SetCamera( SceneDescriptor.camera );
	quad.Bind();

	OpenGLState::EnableStencilTest( false );
	OpenGLState::SetCullFaceType( CT_BACK );
	OpenGLState::EnableBlend( true );

	for ( UInt32_t indexLight = 0, countLights = SceneDescriptor.directionalLights.size(); indexLight < countLights; ++indexLight )
	{
		auto*			light = SceneDescriptor.directionalLights[ indexLight ];
		
		shaderLighting.SetLight( light );
		shaderLighting.Bind();	
		glDrawElements( GL_TRIANGLES, quad.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( quad.GetStartIndex() * sizeof( UInt32_t ) ) );
	}

	OpenGLState::EnableBlend( false );
	OpenGLState::EnableStencilTest( false );
	OpenGLState::EnableDepthTest( true );
	OpenGLState::EnableDepthWrite( true );
}

// ------------------------------------------------------------------------------------ //
// Финальный проход Deffered Shading'a
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Render_FinalPass( const SceneDescriptor& SceneDescriptor ) 
{
	gbuffer.ShowFinalFrame();
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
