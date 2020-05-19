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
#include "materialproxyfactory.h"
#include "animatedtextureproxy.h"

// ------------------------------------------------------------------------------------ //
// Register material proxy
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyFactory::Register( const char* Name, CreateMaterialProxyFn_t CreateMaterialProxy )
{
	char*			name = new char[ strlen( Name ) ];
	strcpy( name, Name );

	nameMaterialProxes.push_back( name );
	materialProxes[ Name ] = CreateMaterialProxy;
}

// ------------------------------------------------------------------------------------ //
// Unregister material proxy
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyFactory::Unregister( const char* Name )
{
	auto		it = materialProxes.find( Name );
	if ( it == materialProxes.end() )		return;

	for ( UInt32_t index = 0, count = nameMaterialProxes.size(); index < count; ++index )
		if ( strcmp( nameMaterialProxes[ index ], Name ) == 0 )
		{
			delete[] nameMaterialProxes[ index ];
			nameMaterialProxes.erase( nameMaterialProxes.begin() + index );
			break;
		}

	materialProxes.erase( it );
}

// ------------------------------------------------------------------------------------ //
// Create material proxy
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy* le::MaterialProxyFactory::Create( const char* Name )
{
	auto		it = materialProxes.find( Name );
	if ( it == materialProxes.end() )		return nullptr;
	return it->second();
}

// ------------------------------------------------------------------------------------ //
// Get count material proxes
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::MaterialProxyFactory::GetCountMaterialProxes() const
{
	return nameMaterialProxes.size();
}

// ------------------------------------------------------------------------------------ //
// Get material proxy
// ------------------------------------------------------------------------------------ //
const char* le::MaterialProxyFactory::GetMaterialProxy( UInt32_t Index ) const
{
	if ( Index >= nameMaterialProxes.size() )		return "unknown";
	return nameMaterialProxes[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get material proxes
// ------------------------------------------------------------------------------------ //
const char** le::MaterialProxyFactory::GetMaterialProxes() const
{
	return ( const char** ) nameMaterialProxes.data();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::MaterialProxyFactory::MaterialProxyFactory()
{
	Register( "AnimatedTexture", []() -> IMaterialProxy* { return new AnimatedTextureProxy(); } );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::MaterialProxyFactory::~MaterialProxyFactory()
{
	for ( UInt32_t index = 0, count = nameMaterialProxes.size(); index < count; ++index )
		delete[] nameMaterialProxes[ index ];
}
