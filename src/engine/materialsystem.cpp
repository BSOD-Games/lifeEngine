//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/material.h"
#include "engine/materialsystem.h"

// ------------------------------------------------------------------------------------ //
// Get material proxy factory
// ------------------------------------------------------------------------------------ //
le::IMaterialProxyFactory* le::MaterialSystem::GetMaterialProxyFactory() const
{
	return ( IMaterialProxyFactory* ) &materialProxyFactory;
}

// ------------------------------------------------------------------------------------ //
// Get shader factory
// ------------------------------------------------------------------------------------ //
le::IShaderFactory* le::MaterialSystem::GetShaderFactory() const
{
	return ( IShaderFactory* ) &shaderFactory;
}

// ------------------------------------------------------------------------------------ //
// Initialize subsystem
// ------------------------------------------------------------------------------------ //
bool le::MaterialSystem::Initialize( IEngine* Engine )
{
	return true;
}

// ------------------------------------------------------------------------------------ //
// Update materials
// ------------------------------------------------------------------------------------ //
void le::MaterialSystem::Update()
{
	if ( updateMaterials.empty() ) return;
	for ( UInt32_t index = 0, count = updateMaterials.size(); index < count; ++index )
	{
		Material*			material = updateMaterials[ index ];
		material->UpdateMaterialProxy();

		if ( material->GetCountReferences() <= 1 )		material->Release();
		else											material->DecrementReference();
	}

	updateMaterials.clear();
}

// ------------------------------------------------------------------------------------ //
// Submit update material on update
// ------------------------------------------------------------------------------------ //
void le::MaterialSystem::SubmitUpdate( IMaterial* Material )
{
	Material->IncrementReference();
	updateMaterials.push_back( static_cast< le::Material* >( Material ) );
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::MaterialSystem::MaterialSystem()
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::MaterialSystem::~MaterialSystem()
{
	if ( updateMaterials.empty() ) return;
	for ( UInt32_t index = 0, count = updateMaterials.size(); index < count; ++index )
	{
		Material*			material = updateMaterials[ index ];

		if ( material->GetCountReferences() <= 1 )		
			material->Release();
		else											
			material->DecrementReference();
	}
}
