//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "engine/lifeengine.h"
#include "engine/imaterialproxy.h"
#include "engine/materialmanager.h"

// ------------------------------------------------------------------------------------ //
// Register proxy-material to manager materials
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::RegisterProxy( IMaterialProxy* MaterialProxy )
{
    proxes.push_back( MaterialProxy );
}

// ------------------------------------------------------------------------------------ //
// Unregister proxy-material from manager materials
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::UnregisterProxy( IMaterialProxy* MaterialProxy )
{
    for ( UInt32_t index = 0, count = proxes.size(); index < count; ++index )
    {
        IMaterialProxy*         proxy = proxes[ index ];
        if ( proxy == MaterialProxy )
        {
            proxes.erase( proxes.begin() + index );
            return;
        }
    }
}

// ------------------------------------------------------------------------------------ //
// Unregister proxy-material from manager materials
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::UnregisterProxy( UInt32_t Index )
{
    LIFEENGINE_ASSERT( Index < proxes.size() );
    proxes.erase( proxes.begin() + Index );
}

// ------------------------------------------------------------------------------------ //
// Clear all proxy-materials from manager
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::ClearProxes()
{
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
le::MaterialManager::MaterialManager()
{

}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::MaterialManager::~MaterialManager()
{

}

// ------------------------------------------------------------------------------------ //
// Update registered proxy-materials in manager
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::Update( UInt32_t DeltaTime )
{
    for ( UInt32_t index = 0, count = proxes.size(); index < count; ++index )
        proxes[ index ]->Update( DeltaTime );
}
