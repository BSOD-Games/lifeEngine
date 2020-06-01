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
		studioRender->SubmitModel( *it );

	// Render lights
	for ( auto it = lights.begin(), itEnd = lights.end(); it != itEnd; ++it )
		studioRender->SubmitLight( *it );

	studioRender->EndScene();
}

// ------------------------------------------------------------------------------------ //
// Update
// ------------------------------------------------------------------------------------ //
void Scene::Update()
{
	for ( auto it = models.begin(), itEnd = models.end(); it != itEnd; ++it )
		( *it )->Rotate( le::Vector3D_t( 0.005f ) );
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
void Scene::AddLight( le::IPointLight* Light )
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
		le::IModel*			model = *it;
		if ( model->GetCountReferences() <= 1 )			model->Release();
		else											model->DecrementReference();
	}

	for ( auto it = lights.begin(), itEnd = lights.end(); it != itEnd; ++it )
	{
		le::IPointLight*			light = *it;
		if ( light->GetCountReferences() <= 1 )			light->Release();
		else											light->DecrementReference();
	}

	lights.clear();
	models.clear();
}