//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_PROXY_H
#define MATERIAL_PROXY_H

#include "engine/imaterialproxy.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class AnimatedTextureProxy : public IMaterialProxy
    {
    public:
        // IMaterialProxy
        virtual void                        SetVar( IMaterialProxyVar* MaterialProxyVar );
        virtual void                        ClearVar( const char* NameVar );
        virtual void                        ClearAllVars();
        virtual void                        Update( UInt32_t DeltaTime );

        virtual const char*                 GetName() const;
        virtual IMaterialProxyVar*          GetVar( const char* NameVar ) const;

        // AnimatedTextureProxy
        AnimatedTextureProxy();
        ~AnimatedTextureProxy();

    private:
        bool                            isInitialized;

        float                           currentFrameFire;

        IMaterialProxyVar*              frames;
        IMaterialProxyVar*              textureRectVar;
        IMaterialProxyVar*              frameRate;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_PROXY_H
