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

#include "proxes/animatedtextureproxy.h"

#include "materialproxyfactory.h"

// ------------------------------------------------------------------------------------ //
// Register material proxy
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyFactory::Register( const MaterialProxyDescriptor& MaterialProxyDescriptor )
{
	le::MaterialProxyDescriptor			materialProxyDescriptor;
	materialProxyDescriptor.CreateMaterialProxyFn = MaterialProxyDescriptor.CreateMaterialProxyFn;
	materialProxyDescriptor.countParameters = MaterialProxyDescriptor.countParameters;
	materialProxyDescriptor.parametersInfo = MaterialProxyDescriptor.parametersInfo;

	materialProxyDescriptor.name = new char[ strlen( MaterialProxyDescriptor.name ) ];
	strcpy( materialProxyDescriptor.name, MaterialProxyDescriptor.name );

	materialProxes.push_back( materialProxyDescriptor );
	materialProxesFind[ materialProxyDescriptor.name ] = materialProxyDescriptor;
}

// ------------------------------------------------------------------------------------ //
// Unregister material proxy
// ------------------------------------------------------------------------------------ //
void le::MaterialProxyFactory::Unregister( const char* Name )
{
	auto		it = materialProxesFind.find( Name );
	if ( it == materialProxesFind.end() )		return;

	for ( UInt32_t index = 0, count = materialProxes.size(); index < count; ++index )
		if ( strcmp( materialProxes[ index ].name, Name ) == 0 )
		{
			delete[] materialProxes[ index ].name;
			materialProxes.erase( materialProxes.begin() + index );
			break;
		}

	materialProxesFind.erase( it );
}

// ------------------------------------------------------------------------------------ //
// Create material proxy
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy* le::MaterialProxyFactory::Create( const char* Name )
{
	auto		it = materialProxesFind.find( Name );
	if ( it == materialProxesFind.end() )		return nullptr;
	return it->second.CreateMaterialProxyFn();
}

// ------------------------------------------------------------------------------------ //
// Get count material proxes
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::MaterialProxyFactory::GetCountMaterialProxes() const
{
	return materialProxes.size();
}

// ------------------------------------------------------------------------------------ //
// Get material proxy
// ------------------------------------------------------------------------------------ //
le::MaterialProxyDescriptor le::MaterialProxyFactory::GetMaterialProxy( UInt32_t Index ) const
{
	if ( Index >= materialProxes.size() )		return MaterialProxyDescriptor();
	return materialProxes[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Get material proxes
// ------------------------------------------------------------------------------------ //
le::MaterialProxyDescriptor* le::MaterialProxyFactory::GetMaterialProxes() const
{
	return ( le::MaterialProxyDescriptor* ) materialProxes.data();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::MaterialProxyFactory::MaterialProxyFactory()
{
	Register( AnimatedTextureProxy::GetDescriptor() );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::MaterialProxyFactory::~MaterialProxyFactory()
{
	for ( UInt32_t index = 0, count = materialProxes.size(); index < count; ++index )
		delete[] materialProxes[ index ].name;
}
