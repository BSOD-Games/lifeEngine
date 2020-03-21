//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "engine/lifeengine.h"
#include "engine/iengine.h"
#include "engine/ifactory.h"
#include "engine/imaterialproxy.h"
#include "engine/consolesystem.h"
#include "engine/materialmanager.h"
#include "engine/animatedtextureproxy.h"
#include "global.h"

// ------------------------------------------------------------------------------------ //
// Register proxy factory
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::RegisterProxyFactory( le::IFactory* MaterialProxyFactory )
{
    gameProxyFactory = MaterialProxyFactory;
}

// ------------------------------------------------------------------------------------ //
// Unregister proxy factory
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::UnregisterProxyFactory()
{
    gameProxyFactory = nullptr;
}

// ------------------------------------------------------------------------------------ //
// Create proxy
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy* le::MaterialManager::CreateProxy( const char* NameProxy )
{
    IMaterialProxy*             proxy = nullptr;

    if ( gameProxyFactory )          proxy = ( IMaterialProxy* ) gameProxyFactory->Create( NameProxy );
    if ( !proxy )
    {
        if ( strcmp( NameProxy, "AnimatedTexture" ) == 0 )          proxy = new AnimatedTextureProxy();
    }

    if ( proxy )
    {
        proxy->IncrementReference();
        proxes.push_back( proxy );
        return proxy;
    }

    g_consoleSystem->PrintError( "Not found proxy-material [%s] in material manager", NameProxy );
    return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Remove proxy
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::RemoveProxy( le::IMaterialProxy* MaterialProxy )
{
    LIFEENGINE_ASSERT( MaterialProxy );

    for ( UInt32_t index = 0, count = proxes.size(); index < count; ++index )
    {
        IMaterialProxy*         proxy = proxes[ index ];
        if ( proxy == MaterialProxy )
        {
            if ( MaterialProxy->GetCountReferences() <= 1 )
                MaterialProxy->Release();
            else
                MaterialProxy->DecrementReference();

            proxes.erase( proxes.begin() + index );
            return;
        }
    }
}

// ------------------------------------------------------------------------------------ //
// Remove all proxes
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::RemoveAllProxes()
{
    for ( auto it = proxes.begin(), itEnd = proxes.end(); it != itEnd; )
        if ( (*it)->GetCountReferences() <= 1 )
        {
            (*it)->Release();

            it = proxes.erase( it );
            itEnd = proxes.end();
        }
        else
            ++it;
}

// ------------------------------------------------------------------------------------ //
// Get count proxes
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::MaterialManager::GetCountProxes() const
{
    return proxes.size();
}

// ------------------------------------------------------------------------------------ //
// Get proxes
// ------------------------------------------------------------------------------------ //
le::IMaterialProxy** le::MaterialManager::GetProxes() const
{
    return ( IMaterialProxy** ) proxes.data();
}

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
le::MaterialManager::MaterialManager() :
    gameProxyFactory( nullptr )
{}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
le::MaterialManager::~MaterialManager()
{
    RemoveAllProxes();
}

// ------------------------------------------------------------------------------------ //
// Update proxes
// ------------------------------------------------------------------------------------ //
void le::MaterialManager::UpdateProxes()
{
    for ( UInt32_t index = 0, count = proxes.size(); index < count; ++index )
    {
        IMaterialProxy*                     proxy = proxes[ index ];
        if ( proxy->IsNeadUpdate() )        proxy->Update();
    }
}

