//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "engine/lifeengine.h"
#include "engine/imaterialproxy.h"
#include "engine/consolesystem.h"
#include "engine/materialmanager.h"

// ------------------------------------------------------------------------------------ //
// Register game factory proxy-materials
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::RegisterGameProxyFactory( IMaterialProxyFactory* MaterialProxyFactory )
{
    gameProxyFactory = MaterialProxyFactory;
}

// ------------------------------------------------------------------------------------ //
// Unregister game factory proxy-materials
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::UnregisterGameProxyFactory()
{
    gameProxyFactory = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Create proxy-material
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy* le::MaterialManager::CreateProxy( const char* NameProxy )
{
    IMaterialProxy*             materialProxy = nullptr;

    if ( gameProxyFactory )
        materialProxy = gameProxyFactory->CreateProxy( NameProxy );

    if ( !materialProxy )
        materialProxy = proxyFactory.CreateProxy( NameProxy );

    if ( materialProxy )
    {
        proxes.push_back( materialProxy );
        return materialProxy;
    }

    g_consoleSystem->PrintError( "Not found proxy-material [%s] in material manager", NameProxy );
    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Delete proxy-material
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::DeleteProxy( IMaterialProxy* MaterialProxy )
{
    for ( UInt32_t index = 0, count = proxes.size(); index < count; ++index )
    {
        IMaterialProxy*         proxy = proxes[ index ];
        if ( proxy == MaterialProxy )
        {
            delete proxy;
            proxes.erase( proxes.begin() + index );
            return;
        }
    }
}

// ------------------------------------------------------------------------------------ //
// Delete proxy-material
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::DeleteProxy( UInt32_t Index )
{
    LIFEENGINE_ASSERT( Index < proxes.size() );
    delete proxes[ Index ];
    proxes.erase( proxes.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Delete all proxy-materials
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::DeleteAllProxes()
{
    for ( UInt32_t index = 0, count = proxes.size(); index < count; ++index )
        delete proxes[ index ];

    proxes.clear();
}

// ------------------------------------------------------------------------------------ //
// Get count proxy-materials registered in manager
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::MaterialManager::GetCountProxes() const
{
    return proxes.size();
}

// ------------------------------------------------------------------------------------ //
// Get array proxy-materials in manager
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy** le::MaterialManager::GetProxes() const
{
    return ( IMaterialProxy** ) proxes.data();
}

// ------------------------------------------------------------------------------------ //
// Get proxy-material from manager
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy* le::MaterialManager::GetProxy( UInt32_t Index ) const
{
    LIFEENGINE_ASSERT( Index < proxes.size() );
    return proxes[ Index ];
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::MaterialManager::MaterialManager() :
    gameProxyFactory( nullptr )
{

}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::MaterialManager::~MaterialManager()
{
    DeleteAllProxes();
}

// ------------------------------------------------------------------------------------ //
// Update registered proxy-materials in manager
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::Update( UInt32_t DeltaTime )
{
    for ( UInt32_t index = 0, count = proxes.size(); index < count; ++index )
        proxes[ index ]->Update( DeltaTime );
}
