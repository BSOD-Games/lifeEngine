//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "scene.h"
<<<<<<< HEAD
#include <qdebug.h>
=======
#include "window_viewer.h"

#include <qdebug.h>
#include <qevent.h>
>>>>>>> 1fb5e962b285d1135187cf6785e4cf94edac1e8c

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Scene::Scene() :
	camera( nullptr )
{
	connect( &timerUpdate, &QTimer::timeout, this, &Scene::Update );
	timerUpdate.start( 5 );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Scene::~Scene()
{
	Clear();
	disconnect( &timerUpdate, &QTimer::timeout, this, &Scene::Update );
	timerUpdate.stop();
}

// ------------------------------------------------------------------------------------ //
// Render
// ------------------------------------------------------------------------------------ //
void Scene::Render()
{
	if ( !studioRender ) return;

	studioRender->BeginScene( camera );

	// Render models
	for ( auto it = models.begin(), itEnd = models.end(); it != itEnd; ++it )
	{
		studioRender->SubmitModel( *it );
		studioRender->SubmitDebugLine( ( *it )->GetMin(), ( *it )->GetMax(), le::Vector3D_t( 1, 1, 1 ) );
	}

	// Render lights
	for ( auto it = lights.begin(), itEnd = lights.end(); it != itEnd; ++it )
		studioRender->SubmitLight( *it );

	// Render ground
	if ( isGround )
	{
		for ( int x = -255; x < 255; x += 20 )
			studioRender->SubmitDebugLine( le::Vector3D_t( x, 0, 0 ), le::Vector3D_t( 255, 0, 0 ), le::Vector3D_t( 1, 1, 1 ) );

		for ( int z = -255; z < 255; z += 20 )
			studioRender->SubmitDebugLine( le::Vector3D_t( 0, 0, z ), le::Vector3D_t( 0, 0, 255 ), le::Vector3D_t( 1, 1, 1 ) );
	}


	studioRender->EndScene();
}

// ------------------------------------------------------------------------------------ //
// Update
// ------------------------------------------------------------------------------------ //
void Scene::Update()
{
	if ( GetIsRotate() )
		camera->Rotate( le::Vector3D_t( 0, 0, 10 ) );
}

// ------------------------------------------------------------------------------------ //
// Add model
// ------------------------------------------------------------------------------------ //
void Scene::AddModel( le::IModel* Model )
{
	if ( !Model ) return;

	Model->IncrementReference();
	models.push_back( Model );
}

// ------------------------------------------------------------------------------------ //
// Add light
// ------------------------------------------------------------------------------------ //
void Scene::AddLight( le::IDirectionalLight* Light )
{
	if ( !Light ) return;

	Light->IncrementReference();
	lights.push_back( Light );
}

// ------------------------------------------------------------------------------------ //
// Set camera
// ------------------------------------------------------------------------------------ //
void Scene::SetCamera( le::ICamera* Camera )
{
	if ( camera == Camera ) return;
	if ( camera )
	{
		if ( camera->GetCountReferences() <= 1 )
			camera->Release();
		else
			camera->DecrementReference();
	}

	camera = Camera;
}

// ------------------------------------------------------------------------------------ //
// Enable or disable ground
// ------------------------------------------------------------------------------------ //
void Scene::EnableGround( bool IsEnable )
{
	isGround = IsEnable;
}

// ------------------------------------------------------------------------------------ //
// Enable or disable rotate
// ------------------------------------------------------------------------------------ //
void Scene::EnableRotate( bool IsEnable )
{
	isRotate = IsEnable;
}

// ------------------------------------------------------------------------------------ //
// Get isGround
// ------------------------------------------------------------------------------------ //
bool Scene::GetIsGround()
{
	return isGround;
}

// ------------------------------------------------------------------------------------ //
// Get isRotate
// ------------------------------------------------------------------------------------ //
bool Scene::GetIsRotate()
{
	return isRotate;
}

// ------------------------------------------------------------------------------------ //
// Clear scene
// ------------------------------------------------------------------------------------ //
void Scene::Clear()
{
	if ( camera )
	{
		if ( camera->GetCountReferences() <= 1 )
			camera->Release();
		else
			camera->DecrementReference();
	}

	for ( auto it = models.begin(), itEnd = models.end(); it != itEnd; ++it )
	{
		le::IModel* model = *it;
		if ( model->GetCountReferences() <= 1 )			model->Release();
		else											model->DecrementReference();
	}

	for ( auto it = lights.begin(), itEnd = lights.end(); it != itEnd; ++it )
	{
		le::IDirectionalLight* light = *it;
		if ( light->GetCountReferences() <= 1 )			light->Release();
		else											light->DecrementReference();
	}

	lights.clear();
	models.clear();
}