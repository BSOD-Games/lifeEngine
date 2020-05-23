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
#include "engine/iengineinternal.h"
#include "engine/iwindow.h"
#include "engine/iconsolesystem.h"
#include "engine/iconvar.h"
#include "engine/imaterial.h"
#include "engine/ishader.h"
#include "settingscontext.h"
#include "common/shaderdescriptor.h"
#include "common/meshsurface.h"
#include "engine/iresourcesystem.h"
#include "global.h"
#include "studiorender.h"
#include "gpuprogram.h"
#include "texture.h"
#include "mesh.h"
#include "openglstate.h"
#include "pointlight.h"
#include "spotlight.h"
#include "directionallight.h"
#include "common/meshdescriptor.h"
#include "studiorender/studiovertexelement.h"
#include "common/shaderdescriptor.h"
#include "engine/iconcmd.h"
#include "engine/icamera.h"
#include "engine/isprite.h"
#include "studiorender/paths.h"

LIFEENGINE_STUDIORENDER_API( le::StudioRender );

static le::IConVar*		r_wireframe = nullptr;
static le::IConVar*		r_showgbuffer = nullptr;

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

	case PT_TRIANGLE_FAN:
		renderObject.primitiveType = GL_TRIANGLE_FAN;
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
	g_resourceSystem = Engine->GetResourceSystem();
	g_engine = Engine;

	if ( !g_consoleSystem )
	{
		g_consoleSystem->PrintError( "Studiorender requared console system" );
		return false;
	}

	// Register console commands and vars
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

	isInitialize = true;
	return true;
}

// ------------------------------------------------------------------------------------ //
// Create render context
// ------------------------------------------------------------------------------------ //
bool le::StudioRender::CreateContext( le::WindowHandle_t WindowHandle, UInt32_t Width, UInt32_t Height )
{
	if ( renderContext.IsCreated() )		return true;
	if ( !isInitialize )
	{
		g_consoleSystem->PrintError( "Studiorender not initialized" );
		return false;
	}

	if ( !WindowHandle )
	{
		g_consoleSystem->PrintError( "Window handle is nullptr" );
		return false;
	}

	g_consoleSystem->PrintInfo( "Creating render context" );

	Configurations				configurations = g_engine->GetConfigurations();
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

	if ( !renderContext.Create( WindowHandle, settingsContext ) )
	{
		g_consoleSystem->PrintError( "Failed created context" );
		return false;
	}

	renderContext.SetVerticalSync( configurations.isVerticalSinc );

	// Initialize OpenGL
	glEnable( GL_TEXTURE_2D );
	OpenGLState::Initialize();
	glPolygonOffset( 1.f, 1.f );

	// Getting configurations videocard
	GLint			tempValue = 0;

	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &tempValue );
	deviceConfigurations.maxTextureSize = tempValue;

	viewport.x = viewport.y = 0;
	viewport.width = Width;
	viewport.height = Height;

	// Initialize GBuffer
	if ( !gbuffer.Initialize( Vector2DInt_t( viewport.width, viewport.height ) ) )
	{
		g_consoleSystem->PrintError( "Failed initialize GBuffer" );
		return false;
	}

	// Initialize primitives
	quad.Create();
	sphere.Create();
	cone.Create();

	if ( !shaderDepth.Create() || !shaderLighting.Create() || !shaderPostrocess.Create() || !shaderDebugPrimitives.Create() )
		return false;

	shaderLighting.SetType( ShaderLighting::LT_POINT );
	shaderLighting.SetSizeViewport( Vector2D_t( viewport.width, viewport.height ) );

	shaderLighting.SetType( ShaderLighting::LT_SPOT );
	shaderLighting.SetSizeViewport( Vector2D_t( viewport.width, viewport.height ) );

	shaderLighting.SetType( ShaderLighting::LT_DIRECTIONAL );
	shaderLighting.SetSizeViewport( Vector2D_t( viewport.width, viewport.height ) );

	shaderPostrocess.SetSizeViewport( Vector2D_t( viewport.width, viewport.height ) );

	// Array verteces line
	float		lineVerteces[] =
	{
		// X	Y		Z
		0.f,	0.f,	0.f,
		0.f,	0.f,	0.f
	};

	// Initialize info by format vertex line
	VertexBufferLayout		vertexFormat;
	vertexFormat.PushFloat( 3 );

	// Create verex buffer for render lines
	vao_DebugPrimitive.Create();
	vbo_DebugPrimitive.Create();

	// Pull data in VBO
	vbo_DebugPrimitive.Bind();
	vbo_DebugPrimitive.Allocate( lineVerteces, sizeof( float ) * 6 );

	// Add VBO to VAO
	vao_DebugPrimitive.AddBuffer( vbo_DebugPrimitive, vertexFormat );
	vao_DebugPrimitive.Unbind();

	g_consoleSystem->PrintInfo( "Craeted render context" );
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

	// Геометрический проход Deffered Shading'a
	glEnable( GL_POLYGON_OFFSET_FILL );
	Render_GeometryPass();
	glDisable( GL_POLYGON_OFFSET_FILL );

	// Проход освещения
	Render_LightPass();

	// Показать финальный кадр
	Render_FinalPass();

	// Render debug lines and points
	if ( isNeedRenderDebugPrimitives )
	{
		gbuffer.CopyDepthBufferToDefaultBuffer();
		shaderDebugPrimitives.Bind();
		vao_DebugPrimitive.Bind();

		for ( UInt32_t indexScene = 0, countScenes = scenes.size(); indexScene < countScenes; ++indexScene )
		{
			SceneDescriptor&				sceneDescriptor = scenes[ indexScene ];

			if ( sceneDescriptor.debugLines.empty() && sceneDescriptor.debugPoints.empty() )
				continue;

			shaderDebugPrimitives.SetProjectionMatrix( sceneDescriptor.camera->GetProjectionMatrix() * sceneDescriptor.camera->GetViewMatrix() );

			// Render lines with help instanced
			for ( UInt32_t indexLine = 0, countLinesInGPU = 0, countLines = sceneDescriptor.debugLines.size(); indexLine < countLines; ++indexLine )
			{
				if ( countLinesInGPU < 255 )
				{
					shaderDebugPrimitives.SetPrimitive( countLinesInGPU, sceneDescriptor.debugLines[ indexLine ] );
					++countLinesInGPU;
				}

				if ( countLinesInGPU == 255 || indexLine + 1 == countLines )
				{
					glDrawArraysInstanced( GL_LINES, 0, 2, countLinesInGPU );
					countLinesInGPU = 0;
				}
			}

			// Render points with help instanced
			for ( UInt32_t indexPoint = 0, countPointsInGPU = 0, countPoints = sceneDescriptor.debugPoints.size(); indexPoint < countPoints; ++indexPoint )
			{
				if ( countPointsInGPU < 255 )
				{
					shaderDebugPrimitives.SetPrimitive( countPointsInGPU, sceneDescriptor.debugPoints[ indexPoint ] );
					++countPointsInGPU;
				}

				if ( countPointsInGPU == 255 || indexPoint + 1 == countPoints )
				{
					glDrawArraysInstanced( GL_POINTS, 0, 1, countPointsInGPU );
					countPointsInGPU = 0;
				}
			}
		}

		shaderDebugPrimitives.Unbind();
		vao_DebugPrimitive.Unbind();
		isNeedRenderDebugPrimitives = false;
	}

	if ( r_showgbuffer->GetValueBool() )		gbuffer.ShowBuffers();
	renderContext.SwapBuffers();
}

// ------------------------------------------------------------------------------------ //
// Геометрический проход Deffered Shading'a
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Render_GeometryPass()
{
	gbuffer.Bind( GBuffer::BT_GEOMETRY );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for ( UInt32_t indexScene = 0, countScenes = scenes.size(); indexScene < countScenes; ++indexScene )
	{
		SceneDescriptor&				sceneDescriptor = scenes[ indexScene ];

		for ( UInt32_t indexObject = 0, countObjects = sceneDescriptor.renderObjects.size(); indexObject < countObjects; ++indexObject )
		{
			const RenderObject&		renderObject = sceneDescriptor.renderObjects[ indexObject ];
			OpenGLState::EnableDepthTest( renderObject.material->IsDepthTest() );
			OpenGLState::EnableDepthWrite( renderObject.material->IsDepthWrite() );
			OpenGLState::EnableBlend( renderObject.material->IsBlend() );
			OpenGLState::EnableCullFace( renderObject.material->IsCullFace() );
			OpenGLState::SetCullFaceType( renderObject.material->GetCullFaceType() );

			renderObject.material->OnBind( renderObject.transformation, sceneDescriptor.camera, renderObject.lightmap );
			renderObject.vertexArrayObject->Bind();
			glDrawRangeElementsBaseVertex( renderObject.primitiveType, 0, renderObject.countIndeces, renderObject.countIndeces, GL_UNSIGNED_INT, ( void* ) ( renderObject.startIndex * sizeof( UInt32_t ) ), renderObject.startVertexIndex );
		}
	}
}

// ------------------------------------------------------------------------------------ //
// Световой проход Deffered Shading'a
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Render_LightPass()
{
	gbuffer.Bind( GBuffer::BT_LIGHT );
	glClear( GL_COLOR_BUFFER_BIT );

	OpenGLState::EnableStencilTest( true );
	OpenGLState::EnableDepthWrite( false );
	OpenGLState::SetCullFaceType( CT_FRONT );
	OpenGLState::SetStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	OpenGLState::SetStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP_EXT, GL_KEEP );

	for ( UInt32_t indexScene = 0, countScenes = scenes.size(); indexScene < countScenes; ++indexScene )
	{
		SceneDescriptor&				sceneDescriptor = scenes[ indexScene ];

		shaderDepth.SetType( ShaderDepth::GT_SPHERE );
		shaderLighting.SetType( ShaderLighting::LT_POINT );
		shaderLighting.SetCamera( sceneDescriptor.camera );
		sphere.Bind();

		for ( UInt32_t indexLight = 0, countLights = sceneDescriptor.pointLights.size(); indexLight < countLights; ++indexLight )
		{
			auto*			light = sceneDescriptor.pointLights[ indexLight ];

			OpenGLState::SetColorMask( false, false, false, false );
			OpenGLState::EnableDepthTest( true );
			glClear( GL_STENCIL_BUFFER_BIT );
			OpenGLState::SetStencilFunc( GL_ALWAYS, 0, 0 );

			shaderDepth.SetPVTMatrix( sceneDescriptor.camera->GetProjectionMatrix() * sceneDescriptor.camera->GetViewMatrix() * light->GetTransformation() );
			shaderDepth.SetRadius( light->GetRadius() );
			shaderDepth.Bind();
			glDrawElements( GL_TRIANGLES, sphere.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( sphere.GetStartIndex() * sizeof( UInt32_t ) ) );

			shaderLighting.SetPVTMatrix( sceneDescriptor.camera->GetProjectionMatrix() * sceneDescriptor.camera->GetViewMatrix() * light->GetTransformation() );
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
		shaderLighting.SetCamera( sceneDescriptor.camera );
		cone.Bind();

		for ( UInt32_t indexLight = 0, countLights = sceneDescriptor.spotLights.size(); indexLight < countLights; ++indexLight )
		{
			auto*			light = sceneDescriptor.spotLights[ indexLight ];

			OpenGLState::SetColorMask( false, false, false, false );
			OpenGLState::EnableDepthTest( true );
			glClear( GL_STENCIL_BUFFER_BIT );
			OpenGLState::SetStencilFunc( GL_ALWAYS, 0, 0 );

			shaderDepth.SetPVTMatrix( sceneDescriptor.camera->GetProjectionMatrix() * sceneDescriptor.camera->GetViewMatrix() * light->GetTransformation() );
			shaderDepth.SetRadius( light->GetRadius() );
			shaderDepth.SetHeight( light->GetHeight() );
			shaderDepth.Bind();
			glDrawElements( GL_TRIANGLES, cone.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( cone.GetStartIndex() * sizeof( UInt32_t ) ) );

			shaderLighting.SetPVTMatrix( sceneDescriptor.camera->GetProjectionMatrix() * sceneDescriptor.camera->GetViewMatrix() * light->GetTransformation() );
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
		shaderLighting.SetCamera( sceneDescriptor.camera );
		quad.Bind();

		OpenGLState::EnableStencilTest( false );
		OpenGLState::SetCullFaceType( CT_BACK );
		OpenGLState::EnableBlend( true );

		for ( UInt32_t indexLight = 0, countLights = sceneDescriptor.directionalLights.size(); indexLight < countLights; ++indexLight )
		{
			auto*			light = sceneDescriptor.directionalLights[ indexLight ];

			shaderLighting.SetLight( light );
			shaderLighting.Bind();
			glDrawElements( GL_TRIANGLES, quad.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( quad.GetStartIndex() * sizeof( UInt32_t ) ) );
		}
	}

	OpenGLState::EnableBlend( false );
	OpenGLState::EnableStencilTest( false );
	OpenGLState::EnableDepthTest( true );
	OpenGLState::EnableDepthWrite( true );
}

// ------------------------------------------------------------------------------------ //
// Финальный проход Deffered Shading'a
// ------------------------------------------------------------------------------------ //
void le::StudioRender::Render_FinalPass()
{
	OpenGLState::SetCullFaceType( CT_BACK );
	OpenGLState::EnableDepthTest( false );

	gbuffer.Bind( GBuffer::BT_FINAL );
	quad.Bind();
	shaderPostrocess.Bind();

	glDrawElements( GL_TRIANGLES, quad.GetCountIndeces(), GL_UNSIGNED_INT, ( void* ) ( quad.GetStartIndex() * sizeof( UInt32_t ) ) );
	OpenGLState::EnableDepthTest( true );
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
// Получить порт вывода
// ------------------------------------------------------------------------------------ //
const le::StudioRenderViewport& le::StudioRender::GetViewport() const
{
	return viewport;
}

// ------------------------------------------------------------------------------------ //
// Get device configurations
// ------------------------------------------------------------------------------------ //
const le::StudioRenderDeviceConfigurations& le::StudioRender::GetDeviceConfigurations() const
{
	return deviceConfigurations;
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
	if ( renderContext.IsCreated() )			renderContext.Destroy();
}

// ------------------------------------------------------------------------------------ //
// Submit debug line
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SubmitDebugLine( const Vector3D_t& From, const Vector3D_t& To, const Vector3D_t& Color )
{
	scenes[ currentScene ].debugLines.push_back( { From, To, Color } );
	isNeedRenderDebugPrimitives = true;
}

// ------------------------------------------------------------------------------------ //
// Submit debug point
// ------------------------------------------------------------------------------------ //
void le::StudioRender::SubmitDebugPoint( const Vector3D_t& Position, const Vector3D_t& Color )
{
	scenes[ currentScene ].debugPoints.push_back( { Position, Color } );
	isNeedRenderDebugPrimitives = true;
}
